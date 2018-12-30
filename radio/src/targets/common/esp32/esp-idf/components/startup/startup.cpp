#include "opentx.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_timer.h"

#define MENUS_STACK_SIZE       2000
#define MIXER_STACK_SIZE       500
#define AUDIO_STACK_SIZE       500
#define MENU_TASK_PERIOD_TICKS      50/portTICK_PERIOD_MS    // 50ms


void menusTask(void * pvParameters)
{
	TickType_t xLastWakeTime;
	const TickType_t xTimeIncrement = MENU_TASK_PERIOD_TICKS;
	
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

#if defined(STM32) && !defined(SIMU)
      if (getSelectedUsbMode() == USB_JOYSTICK_MODE) {
        usbJoystickUpdate();
      }
#endif

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
    }
  }
}


void tasksStarts(void *p)
{
	TaskHandle_t xMenusTaskHandle = NULL;
	TaskHandle_t xMixerTaskHandle = NULL;

    xTaskCreate( menusTask, "menusTask", MENUS_STACK_SIZE, NULL, tskIDLE_PRIORITY, &xMenusTaskHandle );
    xTaskCreate( mixerTask, "menusTask", MIXER_STACK_SIZE, NULL, tskIDLE_PRIORITY, &xMixerTaskHandle );
}	


extern "C" { 
  
void ESPOpentxStart(){
	boardInit();
	tasksStart();
	while(1);
}

}