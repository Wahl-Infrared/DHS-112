#include "display.h"
#include "system.h"
#include "settings.h"

u8 sys_state;

void run_process(){
  float temp;  
  while(sys_state == SYS_STATE_RUN){
    temp = calc_temperature();
    display_main_temp((int)(temp * 10));
    // 检查按键状态
  }
}

void sys_process(){
  while(1){
    switch(sys_state){
      case SYS_STATE_RUN:
        display_run_page(p_setting->ems, p_setting->flags_bit.cf, p_setting->flags_bit.min);
        run_process();
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
}

void set_sys_state(u8 state)
{
  sys_state = state;
}

u8 get_sys_state()
{
  return sys_state;
}

void sys_init()
{
  sys_state = SYS_STATE_RUN;
}

