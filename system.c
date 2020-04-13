#include "gpio.h"
#include "lcd.h"
#include "system.h"
#include "therm_sensor.h"
#include "flash.h"
#include "settings.h"
#include "button.h"

u8 sys_state;

void sys_process(){
  switch(sys_state){
    case SYS_STATE_RUN:      
  //     run_temp_detect();
      break;
    case SYS_STATE_SET_PROCESS:
      set_process();
      break;

    case SYS_STATE_HOLD:
      display_hold_flag();
    default:
      // LPM
      break;
  }
}

void set_sys_state(u8 state)
{
  sys_state = state;
}

u8 get_sys_state()
{
  return sys_state;
}

