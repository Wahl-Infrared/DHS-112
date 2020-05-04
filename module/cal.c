#include "string.h"
#include "display.h"
#include "flash.h"
#include "button.h"
#include "sd16.h"
#include "sensor.h"
#include "cal.h"
#include "settings.h"

#define DEFAULT_GAIN 4
#define DEFAULT_R_P 1000
#define DEFAULT_VAL_ENV 1580

CALIBRATION calibration;
P_CALIBRATION p_calibration = &calibration;

enum{
  CAL_STEP_INIT,
  CAL_STEP_1,
  CAL_STEP_2,
  CAL_STEP_MAX
};

u8 cal_check()
{
  CAL_CHECK_OUT = 0;
  if(CAL_CHECK_IN != 0)return 0;
  CAL_CHECK_OUT = 1;
  if(CAL_CHECK_IN != 1)return 0;
  CAL_CHECK_OUT = 0;
  if(CAL_CHECK_IN != 0)return 0;
  CAL_CHECK_OUT = 1;
  if(CAL_CHECK_IN != 1)return 0;
  CAL_CHECK_OUT = 0;
  if(CAL_CHECK_IN != 0)return 0;
  CAL_CHECK_OUT = 1;
  if(CAL_CHECK_IN != 1)return 0;

  return 1;
}

void cal_init()
{
   u8 buff[MAX_CAL_SIZE];
    u8 crc;
    u8 len = sizeof(CALIBRATION);
  
    /* 读取主区数据，并校验CRC，成功直接返�?*/
    if(read_flash(CAL_ADDR, buff, len+1) == 0){
      crc = calc_crc8(buff, len);
      if(crc == buff[len]){
        memcpy(p_calibration, buff, len);
        return;
      }
    }
  
    /*读取主区数据，并校验CRC，成功直接返�?*/
    if(read_flash(CAL_BAK_ADDR, buff, len+1) == 0){
      crc = calc_crc8(buff, len);
      if(crc == buff[len]){
        memcpy(p_calibration, buff, len);
        return;
      }
    }
  
  
    /* 使用默认设置 */
    p_calibration->gain = DEFAULT_GAIN;
    p_calibration->R_thermistor_plug_up = DEFAULT_R_P;
    p_calibration->thermopile_val_env = DEFAULT_VAL_ENV;
}


void cal_records(u8 step, u16 *val, int set_val)
{
  if(step == CAL_STEP_1){
      p_calibration->thermopile_val_env = *val;
      p_calibration->R_thermistor_plug_up = calc_R_p(*(val+1), set_val);
  }else if(step == CAL_STEP_2){
      p_calibration->gain = calc_gain(val, set_val);
  }

  u8 buff[MAX_SETTING_SIZE];
  u8 len = sizeof(CALIBRATION);

  memcpy(buff, p_calibration, len);
  buff[len] = calc_crc8((u8 *)p_calibration, len);
 
  write_flash(CAL_ADDR, buff, len+1);
  write_flash(CAL_BAK_ADDR, buff, len+1);
}

void cal_process(void)
{
  float temp;
  u8 cal_step = CAL_STEP_1;
  u8 button_state;
  u16 sd16_val[2];
  u16 max_val[2] = {0};
  u16 set_val;

  display_run_page(p_setting->ems, p_setting->flags_bit.cf, 0);

  while(1){
    button_state = get_button_state(BUTTON_NAME_POWER);
    if(button_state == BUTTON_STATE_CLICK){
      clear_hold_flag();
      get_sd16_val(sd16_val);
      if(sd16_val[0] > max_val[0]){
        max_val[0] = sd16_val[0];
        max_val[1] = sd16_val[1];

        temp = calc_temperature(sd16_val);
        display_slave_temp((int)(temp * 10));
      }
      
      temp = calc_temperature(sd16_val);
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
      cal_records(cal_step, max_val, set_val);
      display_cal_records(cal_step);
      cal_step++;
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

