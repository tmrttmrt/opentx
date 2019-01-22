#include <stdio.h>
#include "esp_types.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_task.h"
#include "soc/timer_group_struct.h"
#include "driver/periph_ctrl.h" 
#include "driver/timer.h"
#include "esp_system.h"
#include "esp_timer.h"
//#define LOG_LOCAL_LEVEL ESP_LOG_DEBUG
#include "esp_log.h"
#include "esp_spiffs.h"
#define HASASSERT
#include "opentx.h"



#define MENUS_STACK_SIZE       0xA00
#define MIXER_STACK_SIZE       0x800
#define AUDIO_STACK_SIZE       0x800
#define AUDIO_PLAY_STACK_SIZE  0xA00
#define PER10MS_STACK_SIZE     0x500
#define ENC_STACK_SIZE         0x300
#define MENU_TASK_PERIOD_TICKS      50/portTICK_PERIOD_MS    // 50ms
#define MENU_TASK_CORE 0
#define MIXER_TASK_CORE 1
#define AUDIO_TASK_CORE 0
#define AUDIO_PLAY_TASK_CORE 1
#define PER10MS_TASK_CORE 0
#define ENC_TASK_CORE 0

static const char *TAG = "startup.cpp";
TaskHandle_t xMenusTaskHandle = NULL;
TaskHandle_t xMixerTaskHandle = NULL;
TaskHandle_t xAudioTaskHandle = NULL;
TaskHandle_t xAudioPlayTaskHandle = NULL;
TaskHandle_t xPer10msTaskHandle = NULL;
TaskHandle_t xEncTaskHandle = NULL;

portMUX_TYPE mixerMux= portMUX_INITIALIZER_UNLOCKED;
SemaphoreHandle_t xAudioSem = NULL;
SemaphoreHandle_t xPer10msSem = NULL;

uint16_t menusStackAvailable()
{
    return uxTaskGetStackHighWaterMark(xMenusTaskHandle);
}

uint16_t mixerStackAvailable()
{
    return uxTaskGetStackHighWaterMark(xMixerTaskHandle);
}

uint16_t per10msStackAvailable()
{
    return uxTaskGetStackHighWaterMark(xPer10msTaskHandle);
}

uint16_t audioStackAvailable()
{
    return uxTaskGetStackHighWaterMark(xAudioTaskHandle);
}


uint16_t getTmr2MHz(){
    return ((uint16_t) esp_timer_get_time())*2;
}

void mixEnterCritical(){
    vTaskEnterCritical(&mixerMux);
}

void mixExitCritical(){
    vTaskExitCritical(&mixerMux);
}


void menusTask(void * pvParameters)
{
    TickType_t xLastWakeTime;
    const TickType_t xTimeIncrement = MENU_TASK_PERIOD_TICKS;
    
    ESP_LOGI(TAG,"Starting menusTask.");
    opentxInit();
    
    xLastWakeTime = xTaskGetTickCount ();
    while (1){
        vTaskDelayUntil( &xLastWakeTime, xTimeIncrement );
        perMain();
    }
    DEBUG_TIMER_STOP(debugTimerPerMain);


#if defined(SIMU)
    if (main_thread_running == 0)
    break;
#endif
}

void mixerTask(void * pdata)
{
    static uint32_t lastRunTime;
    s_pulses_paused = true;
    ESP_LOGI(TAG,"Starting mixerTask.\n");
    while(1) {

#if defined(SIMU)
        if (main_thread_running == 0)
        return;
#endif

#if defined(SBUS)
        processSbusInput();
#endif

        vTaskDelay(1);//1 tick 
        //    if (isForcePowerOffRequested()) {
        //      pwrOff();
        //    }
        
        uint32_t now = esp_timer_get_time()/1000;
        bool run = false;
        if ((now - lastRunTime) >= 20) {     // run at least every 20ms
            run = true;
        }
        else if (now >= nextMixerEndTime - 2) {
            run = true;
        }
        if (!run) {
            continue;  // go back to sleep
        }

        lastRunTime = now;
        if (!s_pulses_paused) {
            int64_t t0 = esp_timer_get_time();

            DEBUG_TIMER_START(debugTimerMixer);
            doMixerCalculations();
            DEBUG_TIMER_START(debugTimerMixerCalcToUsage);
            DEBUG_TIMER_SAMPLE(debugTimerMixerIterval);
            DEBUG_TIMER_STOP(debugTimerMixer);

#if defined(TELEMETRY_FRSKY) || defined(TELEMETRY_MAVLINK)
            DEBUG_TIMER_START(debugTimerTelemetryWakeup);
            telemetryWakeup();
            DEBUG_TIMER_STOP(debugTimerTelemetryWakeup);
#endif

#if defined(BLUETOOTH)
            bluetoothWakeup();
#endif

            if (heartbeat == HEART_WDT_CHECK) {
                wdt_reset();
                heartbeat = 0;
            }

            t0 = esp_timer_get_time() - t0;
            if (t0 > maxMixerDuration) maxMixerDuration = t0 ;
            ESP_LOGD(TAG,"maxMixerDuration %d",maxMixerDuration);
        }
    }
}


void  per10msTask(void * pdata){
    while(1){
        xSemaphoreTake(xPer10msSem, portMAX_DELAY);
        per10ms();
    }
}

void tasksStart()
{
    BaseType_t ret;

    ESP_LOGI(TAG,"Starting tasks.");

    ret=xTaskCreatePinnedToCore( menusTask, "menusTask", MENUS_STACK_SIZE, NULL, ESP_TASK_PRIO_MAX -9, &xMenusTaskHandle, MENU_TASK_CORE );
    configASSERT( xMenusTaskHandle );
    
    ret=xTaskCreatePinnedToCore( mixerTask, "mixerTask", MIXER_STACK_SIZE, NULL, ESP_TASK_PRIO_MAX -6, &xMixerTaskHandle, MIXER_TASK_CORE );
    configASSERT( xMixerTaskHandle );
    
    xAudioSem = xSemaphoreCreateMutex();
    if( xAudioSem != NULL )
    {
        ret=xTaskCreatePinnedToCore( audioTask, "audioTask", AUDIO_STACK_SIZE, NULL, ESP_TASK_PRIO_MAX -8, &xAudioTaskHandle, AUDIO_TASK_CORE );
        configASSERT( xAudioTaskHandle );
        ret=xTaskCreatePinnedToCore( audioPlayTask, "audioPlayTask", AUDIO_PLAY_STACK_SIZE, NULL, ESP_TASK_PRIO_MAX -7, &xAudioPlayTaskHandle, AUDIO_PLAY_TASK_CORE );
        configASSERT( xAudioPlayTaskHandle );
    }
    
    ret=xTaskCreatePinnedToCore( per10msTask, "per10msTask", PER10MS_STACK_SIZE, NULL, ESP_TASK_PRIO_MAX -5, &xPer10msTaskHandle, PER10MS_TASK_CORE );
    configASSERT( xPer10msTaskHandle );
    
    ret=xTaskCreatePinnedToCore( encoderTask, "encoderTask", ENC_STACK_SIZE, NULL, ESP_TASK_PRIO_MAX -4, &xEncTaskHandle, ENC_TASK_CORE );
    configASSERT( xEncTaskHandle );
}

void IRAM_ATTR timer_group0_isr(void *para)
{
    int timer_idx = (int) para;

    /* Retrieve the interrupt status and the counter value
    from the timer that reported the interrupt */
    uint32_t intr_status = TIMERG0.int_st_timers.val;
    TIMERG0.hw_timer[timer_idx].update = 1;

    /* Clear the interrupt
    and update the alarm time for the timer with reload */
    if ((intr_status & BIT(timer_idx)) && timer_idx == TIMER_0) {
        TIMERG0.int_clr_timers.t0 = 1;
    }
    else if ((intr_status & BIT(timer_idx)) && timer_idx == TIMER_1) {
        TIMERG0.int_clr_timers.t1 = 1;
    }
    
    /* After the alarm has been triggered
    we need enable it again, so it is triggered the next time */
    TIMERG0.hw_timer[timer_idx].config.alarm_en = TIMER_ALARM_EN;

    BaseType_t mustYield=false;
    xSemaphoreGiveFromISR(xPer10msSem, &mustYield);
    if (mustYield) portYIELD_FROM_ISR();
}

static void tg0_timer_init(timer_idx_t timer_idx)
{
    /* Select and initialize basic parameters of the timer */
    timer_config_t config;
    memset(&config, 0, sizeof(config));
    config.divider = 16; // TIMER_BASE_CLK/16
    config.counter_dir = TIMER_COUNT_UP;
    config.counter_en = TIMER_PAUSE;
    config.alarm_en = TIMER_ALARM_EN;
    config.intr_type = TIMER_INTR_LEVEL;
    config.auto_reload = 1;
    timer_init(TIMER_GROUP_0, timer_idx, &config);

    /* Timer's counter will initially start from value below.
    Also, if auto_reload is set, this value will be automatically reload on alarm */
    timer_set_counter_value(TIMER_GROUP_0, timer_idx, 0x00000000ULL);

    /* Configure the alarm value and the interrupt on alarm. */
    timer_set_alarm_value(TIMER_GROUP_0, timer_idx, TIMER_BASE_CLK/(16*100)); //100Hz
    timer_enable_intr(TIMER_GROUP_0, timer_idx);
    timer_isr_register(TIMER_GROUP_0, timer_idx, timer_group0_isr, 
    (void *) timer_idx, ESP_INTR_FLAG_IRAM, NULL);

    xPer10msSem = xSemaphoreCreateBinary();
    timer_start(TIMER_GROUP_0, timer_idx);
}

void timer10msInit(){
    ESP_LOGI(TAG,"Starting 10ms timer.");
    tg0_timer_init(TIMER_0); //10 ms interrupt
}

void espLogI(const char * format, ...){
    va_list arglist;
    #define PRINTF_BUFFER_SIZE 255
    char tmp[PRINTF_BUFFER_SIZE];
    va_start(arglist, format);
    vprintf( format, arglist);
    va_end(arglist);    
}

extern "C" void initWiFi();

int main();

extern "C"   void app_main(){
    main();
//    initFS();
//    initWiFi();
    TaskHandle_t tasks[]={xMenusTaskHandle,xMixerTaskHandle,xAudioTaskHandle,xAudioPlayTaskHandle,xPer10msTaskHandle,xEncTaskHandle};
    uint8_t nTasks= sizeof(tasks)/sizeof(tasks[0]);
    while(1){
        ESP_LOGD(TAG,"s_pulses_paused: %d",s_pulses_paused);
        for(uint8_t i=0; i< nTasks; i++){
            ESP_LOGD(TAG,"Min stack: %s: %d",pcTaskGetTaskName(tasks[i]),uxTaskGetStackHighWaterMark(tasks[i]));
        }
        ESP_LOGD(TAG,"maxMixerDuration %d",maxMixerDuration);
        vTaskDelay(1000/portTICK_PERIOD_MS);
    };
}
