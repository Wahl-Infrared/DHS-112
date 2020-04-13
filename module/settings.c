#include "flash.h"
#include "string.h"
#include "button.h"
#include "system.h"
#include "settings.h"

#define   SETTING_ADDR  (0x1000)
#define   SETTING_BAK_ADDR  (0x1080)
#define   MAX_SETTING_SIZE  (64)


//#define   FLASH_CAL_ADDR  (0x1040)
//#define   FLASH_CAL_COPY_ADDR  (0x10C0)

SETTING settings;
PSETTING p_setting = &settings;

void settings_init()
{
  u8 buff[MAX_SETTING_SIZE];
  u8 crc;
  u8 len = sizeof(SETTING);

  flash_init();

  /* 读取主区数据，并校验CRC，成功直接返�?*/
  if(read_flash(SETTING_ADDR, buff, len+1) == 0){
    crc = calc_crc8(buff, len);
    if(crc == buff[len]){
      memcpy(p_setting, buff, len);
      return;
    }
  }

  /*读取主区数据，并校验CRC，成功直接返�?*/
  if(read_flash(SETTING_BAK_ADDR, buff, len+1) == 0){
    crc = calc_crc8(buff, len);
    if(crc == buff[len]){
      memcpy(p_setting, buff, len);
      return;
    }
  }


  /* 使用默认设置 */
  p_setting->hi_temp = HI_TEMP_LIMIT;
  p_setting->lo_temp = LO_TEMP_LIMIT;
  p_setting->ems = HI_EMS_LIMIT;
  p_setting->flags = 0;
}

void save_settings()
{
  u8 buff[MAX_SETTING_SIZE];
  u8 crc;
  u8 len = sizeof(SETTING);

  memcpy(buff, p_setting, len);
  buff[len] = calc_crc8((u8 *)p_setting, len);
 
  write_flash(SETTING_ADDR, buff, len+1);
  write_flash(SETTING_BAK_ADDR, buff, len+1);
}


void disp_step_page(u8 step)
{
  switch(step){
    case STEP_SET_EMS:
      display_set_ems(p_setting->ems);
      break;
    case STEP_SET_HI:
      display_set_hi(p_setting->hi_temp);
      break;
    case STEP_SET_LO:
      display_set_lo(p_setting->lo_temp);
      break;
    case STEP_SET_CF_MIN:
      display_set_cf_min(p_setting->flags_bit.cf, p_setting->flags_bit.min);
      break;
    default:
      break;
  }
}

void up_set_val(u8 step, u8 val)
{
  u8 ems;
  int temp;

  switch(step){
    case STEP_SET_EMS:
      ems = p_setting->ems + val;
      p_setting->ems = ems>HI_EMS_LIMIT ? LO_EMS_LIMIT : ems;
      display_EMS(p_setting->ems);
      break;

    case STEP_SET_HI:
      temp = p_setting->hi_temp + val;      
      p_setting->hi_temp = temp>HI_TEMP_LIMIT?HI_TEMP_LIMIT : temp;
      display_slave_int(p_setting->hi_temp);
      break;
    case STEP_SET_LO:
      temp = p_setting->lo_temp + val;
      p_setting->lo_temp = temp>HI_TEMP_LIMIT?HI_TEMP_LIMIT : temp;
      display_slave_int(p_setting->lo_temp);
      break;
    case STEP_SET_CF_MIN:     
        p_setting->flags_bit.cf = !p_setting->flags_bit.cf;
        display_set_cf_min(p_setting->flags_bit.cf, p_setting->flags_bit.min);
      break;
  }
}

void down_set_val(u8 step, u8 val)
{
  u8 ems;
  int temp;

  switch(step){
    case STEP_SET_EMS:
      ems = p_setting->ems - val;
      p_setting->ems = ems>HI_EMS_LIMIT ? HI_EMS_LIMIT : ems;
      display_EMS(p_setting->ems);
      break;

    case STEP_SET_HI:
      temp = p_setting->hi_temp - val;
      p_setting->hi_temp = temp<LO_TEMP_LIMIT?LO_TEMP_LIMIT : temp;
      display_slave_int(p_setting->hi_temp);
      break;
    case STEP_SET_LO:
      temp = p_setting->lo_temp - val;
      p_setting->lo_temp = temp<LO_TEMP_LIMIT?LO_TEMP_LIMIT : temp;
      display_slave_int(p_setting->lo_temp);
      break;
    case STEP_SET_CF_MIN:
      p_setting->flags_bit.min = !p_setting->flags_bit.min;
      display_set_cf_min(p_setting->flags_bit.cf, p_setting->flags_bit.min);
      break;
  }
}

void set_process()
{
  u8 set_step = 0;
  u8 button_state;
  
  while(get_sys_state() == SYS_STATE_SET_PROCESS && set_step < STEP_MAX){
    button_state = get_button_state(BUTTON_NAME_MENU);
    if(button_state == BUTTON_STATE_CLICK){
       set_step++;
       disp_step_page(set_step);
       set_button_state(BUTTON_NAME_MENU, BUTTON_STATE_INIT);
       continue;
    }

    button_state = get_button_state(BUTTON_NAME_UP);
    if(button_state == BUTTON_STATE_CLICK){
      up_set_val(set_step, 1);
      set_button_state(BUTTON_NAME_UP, BUTTON_STATE_CLICK_ACT);
    }else if(button_state == BUTTON_STATE_PRESSED_1S){
      up_set_val(set_step, 1);
      set_button_state(BUTTON_NAME_UP, BUTTON_STATE_PRESSED_1S_ACT);
    }else if(button_state == BUTTON_STATE_PRESSED_3S){
      up_set_val(set_step, 5);
      set_button_state(BUTTON_NAME_UP, BUTTON_STATE_PRESSED_3S_ACT);
    }else if(button_state == BUTTON_STATE_PRESSED_5S){
      up_set_val(set_step, 10);
      set_button_state(BUTTON_NAME_UP, BUTTON_STATE_PRESSED_5S_ACT);
    }
      
    button_state = get_button_state(BUTTON_NAME_DOWN);
    if(button_state == BUTTON_STATE_CLICK){
      down_set_val(set_step, 1);
      set_button_state(BUTTON_NAME_UP, BUTTON_STATE_CLICK_ACT);
    }else if(button_state == BUTTON_STATE_PRESSED_1S){
      down_set_val(set_step, 1);
      set_button_state(BUTTON_NAME_UP, BUTTON_STATE_PRESSED_1S_ACT);
    }else if(button_state == BUTTON_STATE_PRESSED_3S){
      down_set_val(set_step, 5);
      set_button_state(BUTTON_NAME_UP, BUTTON_STATE_PRESSED_3S_ACT);
    }else if(button_state == BUTTON_STATE_PRESSED_5S){
      down_set_val(set_step, 10);
      set_button_state(BUTTON_NAME_UP, BUTTON_STATE_PRESSED_5S_ACT);
    }
      //低功耗模�?     
  }

  save_settings();
}

