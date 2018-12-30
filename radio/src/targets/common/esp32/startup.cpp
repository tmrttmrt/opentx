#include "opentx.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void ESPOpentxStart();

extern "C" { 
void ESPOpentxStartC(){
	ESPOpentxStart();
}
}

void menusTask(void * pdata)
{
  opentxInit();

    perMain();
    DEBUG_TIMER_STOP(debugTimerPerMain);
    // TODO remove completely massstorage from sky9x firmware
    uint32_t runtime = ((uint32_t)CoGetOSTime() - start);
    // deduct the thread run-time from the wait, if run-time was more than
    // desired period, then skip the wait all together
    if (runtime < MENU_TASK_PERIOD_TICKS) {
      CoTickDelay(MENU_TASK_PERIOD_TICKS - runtime);
    }


#if defined(SIMU)
    if (main_thread_running == 0)
      break;
#endif
  }
  
void tasksStart()
{
}	
  
  void ESPOpentxStart(){
	boardInit();
}
