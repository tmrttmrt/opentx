#include <stdio.h>
#include "esp_types.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "soc/timer_group_struct.h"
#include "driver/periph_ctrl.h" 
#include "driver/timer.h"
#include "esp_system.h"
#include "esp_timer.h"
#include "esp_log.h"
#include "esp_spiffs.h"
#define HASASSERT
#include "opentx.h"



#define MENUS_STACK_SIZE       2000
#define MIXER_STACK_SIZE       2000
#define AUDIO_STACK_SIZE       500
#define MENU_TASK_PERIOD_TICKS      50/portTICK_PERIOD_MS    // 50ms
#define MENU_TASK_CORE 0
#define MIXER_TASK_CORE 1

static const char *TAG = "startup.cpp";
TaskHandle_t xMenusTaskHandle = NULL;
TaskHandle_t xMixerTaskHandle = NULL;

uint16_t getTmr1MHz(){
    return (uint16_t) xTaskGetTickCount ();
}

void menusTask(void * pvParameters)
{
    TickType_t xLastWakeTime;
    const TickType_t xTimeIncrement = MENU_TASK_PERIOD_TICKS;
    
    ESP_LOGI(TAG,"Starting menusTask.\n");
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

uint32_t nextMixerTime[NUM_MODULES];

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

        vTaskDelay(2/portTICK_PERIOD_MS);
        //    if (isForcePowerOffRequested()) {
        //      pwrOff();
        //    }
        
        TickType_t now = xTaskGetTickCount ();
        bool run = false;
        if ((now - lastRunTime) >= 20) {     // run at least every 20ms
            run = true;
        }
        else if (now == nextMixerTime[0]) {
            run = true;
        }
#if NUM_MODULES >= 2
        else if (now == nextMixerTime[1]) {
            run = true;
        }
#endif
        if (!run) {
            continue;  // go back to sleep
        }

        lastRunTime = now;

        if (!s_pulses_paused) {
            int64_t t0 = esp_timer_get_time();

            DEBUG_TIMER_START(debugTimerMixer);
            //      CoEnterMutexSection(mixerMutex);
            doMixerCalculations();
            DEBUG_TIMER_START(debugTimerMixerCalcToUsage);
            DEBUG_TIMER_SAMPLE(debugTimerMixerIterval);
            //      CoLeaveMutexSection(mixerMutex);
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
            ESP_LOGI(TAG,"maxMixerDuration %d",maxMixerDuration);
        }
    }
}


void otxTasksStart()
{
    BaseType_t ret;
    
    ret=xTaskCreatePinnedToCore( menusTask, "menusTask", MENUS_STACK_SIZE, NULL, tskIDLE_PRIORITY, &xMenusTaskHandle, MENU_TASK_CORE );
    ESP_LOGI(TAG,"xTaskCreatePinnedToCore: ret = %d.",ret);
    ret=xTaskCreatePinnedToCore( mixerTask, "mixerTask", MIXER_STACK_SIZE, NULL, tskIDLE_PRIORITY, &xMixerTaskHandle, MIXER_TASK_CORE );
    ESP_LOGI(TAG,"xTaskCreatePinnedToCore: ret = %d.",ret);
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

    /* Now just send the event data back to the main program task */
    //    xQueueSendFromISR(timer_queue, &evt, NULL);
    per10ms();
}

static void tg0_timer_init(timer_idx_t timer_idx)
{
    /* Select and initialize basic parameters of the timer */
    timer_config_t config;
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

    timer_start(TIMER_GROUP_0, timer_idx);
}


uint16_t stackAvailable()
{
    return uxTaskGetStackHighWaterMark(xMenusTaskHandle);
}


extern "C"   void app_main(){
    boardInit();
    ESP_LOGI(TAG,"Starting 10ms timer.");
    tg0_timer_init(TIMER_0); //10 ms interrupt
    ESP_LOGI(TAG,"Starting tasks.");
    otxTasksStart();
    uint8_t buff[64];
    eepromReadBlock(buff, 0, 64);
    eepromReadBlock(buff, 128, 64);
    while(1){
        vTaskDelay(1000/portTICK_PERIOD_MS);
    };
}
