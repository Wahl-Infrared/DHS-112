#include "display.h"
#include "sd16.h"
#include "common.h"
#include "sensor.h"
#include "button.h"
#include "cal.h"
#include "settings.h"

void env_test_process(void)
{
  float temp;
  u8 cal_step = 0;
  u8 button_state;
  u16 sd16_val[2];
  u16 max_val[2] = {0};
  int set_val;

  display_cal_run_page(p_setting->ems, p_setting->flags_bit.cf, 0);

  while(1){
    button_state = get_button_state(BUTTON_NAME_POWER);
    if(button_state == BUTTON_STATE_CLICK){
      clear_hold_flag();
      get_sd16_val(sd16_val);
   
      temp = calc_evn_temp(sd16_val[1]);
      set_val = (int)(temp * 10);
      display_main_temp(set_val);
      continue;
    }else{
      display_hold_flag();
    //低功耗模�?      
    }

    button_state = get_button_state(BUTTON_NAME_MENU);
    if(button_state == BUTTON_STATE_CLICK){       
      set_button_state(BUTTON_NAME_MENU, BUTTON_STATE_CLICK_ACT);
      cal_step++;
      cal_records(cal_step, sd16_val, set_val);
      display_cal_records(cal_step);
      continue;
    }

    button_state = get_button_state(BUTTON_NAME_UP);
    if(button_state == BUTTON_STATE_CLICK){      
      set_button_state(BUTTON_NAME_UP, BUTTON_STATE_CLICK_ACT);
      set_val++;
      display_main_temp(set_val);
    }else if(button_state == BUTTON_STATE_PRESSED_1S){
      set_button_state(BUTTON_NAME_UP, BUTTON_STATE_PRESSED_1S_ACT);
      set_val++;
      display_main_temp(set_val);
    }else if(button_state == BUTTON_STATE_PRESSED_3S){
      set_button_state(BUTTON_NAME_UP, BUTTON_STATE_PRESSED_3S_ACT);
      set_val += 5;
      display_main_temp(set_val);
    }else if(button_state == BUTTON_STATE_PRESSED_5S){
      set_button_state(BUTTON_NAME_UP, BUTTON_STATE_PRESSED_5S_ACT);
      set_val += 10;
      display_main_temp(set_val);
    }
      
    button_state = get_button_state(BUTTON_NAME_DOWN);
    if(button_state == BUTTON_STATE_CLICK){
      set_button_state(BUTTON_NAME_UP, BUTTON_STATE_CLICK_ACT);
      set_val--;
      display_main_temp(set_val);
    }else if(button_state == BUTTON_STATE_PRESSED_1S){
      set_button_state(BUTTON_NAME_UP, BUTTON_STATE_PRESSED_1S_ACT);
      set_val--;
      display_main_temp(set_val);
    }else if(button_state == BUTTON_STATE_PRESSED_3S){
      set_button_state(BUTTON_NAME_UP, BUTTON_STATE_PRESSED_3S_ACT);
      set_val -= 5;
      display_main_temp(set_val);
    }else if(button_state == BUTTON_STATE_PRESSED_5S){
      set_button_state(BUTTON_NAME_UP, BUTTON_STATE_PRESSED_5S_ACT);
      set_val -= 10;
      display_main_temp(set_val);
    }
  }
}

