#include "gpio.h"
#include "lcd.h"
#include "system.h"
#include "therm_sensor.h"
#include "flash.h"

unsigned char sys_cur_state = 0;
unsigned char sys_new_state;
unsigned char sys_settings_changed = 0;
SYSTEM_SETTING system_setting;
PSYSTEM_SETTING p_system_setting = &system_setting;


void set_default_system_settings()
{
  p_system_setting->hi_temp = 1000;
  p_system_setting->lo_temp = -200;
  p_system_setting->ems = 95;
  p_system_setting->max_min = 0;
  p_system_setting->c_f = 0;
  p_system_setting->laser_on = 0;
  p_system_setting->light_on = 0;
  p_system_setting->temp_detect_on = 1;
  return;
}

/*摄氏转华氏*/
void C_2_F(int *val)
{
  int temp = *val;

  temp = 32 + (int)(temp * 1.8);

  *val = temp;
}

/*华氏转摄氏*/
void F_2_C(int *val)
{
  int temp = *val;

  temp = (int)((temp - 32)/1.8);

  *val = temp;
}


/*开关蜂鸣器*/
void bell_on(bool on)
{
  BELL_CTL = on;
}


/*开关激光器*/
void laser_on(bool on)
{
  LASER_CTL = on;
  LCD_flag_display(LCD_FLAG_LASER, on);
}

/*开关LCD 背景灯 */
void light_on(bool on)
{
  LIGHT_CTL = on;
  LCD_flag_display(LCD_FLAG_SUN, on);
}

void main_disp_temp(int temp)
{
  if(temp < -99 || temp > 1000){
    LCD_main_display(temp/10,0);
  }else{
    LCD_main_display(temp,1);
  }
}

void slave_disp_temp(int temp)
{
  if(temp < -99 || temp > 1000){
    LCD_slave_display(temp/10,0);
  }else{
    LCD_slave_display(temp,1);
  }
}

void system_state_change_notify()
{

  LCD_Clear();
  switch(sys_new_state){
    case SYS_STATE_RUN:
      LCD_flag_display(LCD_FLAG_EMS_FLAG, 1);
      LCD_flag_display(LCD_FLAG_EMS_DOT, 1);
      LCD_flag_display(LCD_FLAG_EMS_EQU, 1);
      LCD_EMS_display(p_system_setting->ems);
      if(p_system_setting->c_f){
        LCD_flag_display(LCD_FLAG_TEMP_F, 1);
      }else{
        LCD_flag_display(LCD_FLAG_TEMP_C, 1);
      }
      if(p_system_setting->max_min){
        LCD_flag_display(LCD_FLAG_MIN, 1);
      }else{
        LCD_flag_display(LCD_FLAG_MAX, 1);
      }
      light_on(p_system_setting->light_on);
      laser_on(p_system_setting->laser_on);
  
      break;
    case SYS_STATE_HOLD:
      main_disp_temp(g_temp_result.therm_temp);
      LCD_flag_display(LCD_FLAG_HOLD, 1);
      LCD_flag_display(LCD_FLAG_EMS_FLAG, 1);
      LCD_flag_display(LCD_FLAG_EMS_DOT, 1);
      LCD_flag_display(LCD_FLAG_EMS_EQU, 1);
      LCD_EMS_display(p_system_setting->ems);
      if(p_system_setting->c_f){
        LCD_flag_display(LCD_FLAG_TEMP_F, 1);
      }else{
        LCD_flag_display(LCD_FLAG_TEMP_C, 1);
      }
      if(p_system_setting->max_min){
        LCD_flag_display(LCD_FLAG_MIN, 1);
        slave_disp_temp(g_temp_result.min_temp);
      }else{
        LCD_flag_display(LCD_FLAG_MAX, 1);
        slave_disp_temp(g_temp_result.max_temp);
      }
      laser_on(0);
      light_on(p_system_setting->light_on);
      break;

    case SYS_STATE_SET_EMS:
      LCD_main_display(1, 0);
      LCD_flag_display(LCD_FLAG_SET, 1);
      LCD_blink_display(LCD_FLAG_EMS_FLAG, 1);
      LCD_flag_display(LCD_FLAG_EMS_EQU, 1);
      LCD_EMS_display(p_system_setting->ems);
      break;
    case SYS_STATE_SET_HI:
      LCD_main_display(2, 0);
      LCD_flag_display(LCD_FLAG_SET, 1);
      LCD_blink_display(LCD_FLAG_HI, 1);
      LCD_slave_display(p_system_setting->hi_temp, 0 );
      break;
    case SYS_STATE_SET_LO:
      LCD_main_display(3, 0);
      LCD_flag_display(LCD_FLAG_SET, 1);
      LCD_blink_display(LCD_FLAG_LO, 1);
      LCD_slave_display(p_system_setting->lo_temp, 0);
      break;
    case SYS_STATE_DISP_SWITCH:
      LCD_main_display(4, 0);
      LCD_flag_display(LCD_FLAG_SET, 1);
      if(p_system_setting->c_f){
        LCD_blink_display(LCD_FLAG_TEMP_F, 1);
        LCD_slave_display(g_temp_result.min_temp, 1);
      }else{
        LCD_blink_display(LCD_FLAG_TEMP_C, 1);
        LCD_slave_display(g_temp_result.max_temp, 1);
      }
      if(p_system_setting->max_min){
        LCD_blink_display(LCD_FLAG_MIN, 1);
      }else{
        LCD_blink_display(LCD_FLAG_MAX, 1);
      }
      break;
    case SYS_STATE_CAL_1:
      LCD_main_display(1, 0);
      LCD_EMS_display(p_system_setting->ems);
      LCD_blink_display(LCD_FLAG_SET, 1);
      LCD_flag_display(LCD_FLAG_TEMP_C, 1);
      LCD_flag_display(LCD_FLAG_MAX, 1);
      break;
    case SYS_STATE_CAL_2:
      LCD_main_display(2, 0);
      LCD_EMS_display(p_system_setting->ems);
      LCD_blink_display(LCD_FLAG_SET, 1);
      LCD_flag_display(LCD_FLAG_TEMP_C, 1);
      LCD_flag_display(LCD_FLAG_MAX, 1);
      break;
    case SYS_STATE_CAL_3:
      LCD_main_display(3, 0);
      LCD_EMS_display(p_system_setting->ems);
      LCD_blink_display(LCD_FLAG_SET, 1);
      LCD_flag_display(LCD_FLAG_TEMP_C, 1);
      LCD_flag_display(LCD_FLAG_MAX, 1);
      break;
    case SYS_STATE_READ_MEM:
      LCD_flag_display(LCD_FLAG_SET, 1);
      LCD_blink_display(LCD_FLAG_MEM, 1);
      break;
        
    case SYS_STATE_MAX:
    default:
      break;
  }
}

/****************************************************************************
* 名    称：flush_LCD_disp()
* 功    能：根据state 刷新lcd  显示内容
* 入口参数：无
* 出口参数：无
* 范    例: 
* 说    明: 使用sem 作为信号量指示中断程序也调用该函数，
*                 执行完成后需要再次刷新
****************************************************************************/
void flush_LCD_disp()
{
  static unsigned char sem = 0;

  if(sem++ > 0){
    return;
  }
  
RE_FLUSH:



  switch(sys_cur_state){
    case SYS_STATE_READ_ENV_TEMP:
      main_disp_temp(g_temp_result.env_temp);
      break;
    case SYS_STATE_READ_THERM:
      LCD_main_display(therm_val.therm_avg/1000,0);
      LCD_slave_display(therm_val.therm_avg%1000,0);
      break;
    case SYS_STATE_READ_ENV:
      LCD_main_display(therm_val.env_avg/1000,0);
      LCD_slave_display(therm_val.env_avg%1000,0);
      break;
    case SYS_STATE_RUN:
      main_disp_temp(g_temp_result.therm_temp);
      if(p_system_setting->max_min){
        slave_disp_temp(g_temp_result.min_temp);
      }else{
        slave_disp_temp(g_temp_result.max_temp);
      }

      if(g_temp_result.therm_temp >= p_system_setting->hi_temp){
        LCD_blink_display(LCD_FLAG_HI, 1);
      }else{
        LCD_blink_display(LCD_FLAG_HI, 0);
      }

      if(g_temp_result.therm_temp <= p_system_setting->lo_temp){
        LCD_blink_display(LCD_FLAG_LO, 1);
      }else{
        LCD_blink_display(LCD_FLAG_LO, 0);
      }

      if(g_temp_result.therm_temp >= p_system_setting->hi_temp
        || g_temp_result.therm_temp <= p_system_setting->lo_temp){
        bell_on(1);
      }else{
        bell_on(0);
      }
      break;
    case SYS_STATE_SET_EMS:
      LCD_EMS_display(p_system_setting->ems);
      break;
    case SYS_STATE_SET_HI:
      slave_disp_temp(p_system_setting->hi_temp);
      break;
    case SYS_STATE_SET_LO:
      slave_disp_temp(p_system_setting->lo_temp);
      break;
    case SYS_STATE_DISP_SWITCH:
      if(p_system_setting->max_min){
        LCD_blink_display(LCD_FLAG_MAX, 0);
        LCD_flag_display(LCD_FLAG_MAX, 0);
        LCD_blink_display(LCD_FLAG_MIN, 1);
      }else{
        LCD_blink_display(LCD_FLAG_MAX, 1);
        LCD_blink_display(LCD_FLAG_MIN, 0);
        LCD_flag_display(LCD_FLAG_MIN, 0);
      }
      if(p_system_setting->c_f){
        LCD_blink_display(LCD_FLAG_TEMP_C, 0);
        LCD_flag_display(LCD_FLAG_TEMP_C, 0);
        LCD_blink_display(LCD_FLAG_TEMP_F, 1);
      }else{
        LCD_blink_display(LCD_FLAG_TEMP_C, 1);
        LCD_blink_display(LCD_FLAG_TEMP_F, 0);
        LCD_flag_display(LCD_FLAG_TEMP_F, 0);
      }
      break;
    case SYS_STATE_CAL_1:
    case SYS_STATE_CAL_2:
    case SYS_STATE_CAL_3:
      main_disp_temp(g_temp_result.therm_temp);
      slave_disp_temp(g_temp_result.max_temp);  
      break;
    case SYS_STATE_READ_MEM:
    default:
      break;
  }

  if(sys_cur_state != sys_new_state){
    system_state_change_notify();
    sys_cur_state = sys_new_state;
  }

  if(--sem > 0){
    goto RE_FLUSH;
  }

}

void sys_event_handler(unsigned char event)
{
  int delta_temp;
  int delta_cal_therm;
  int delta_dtc_therm;

  switch(event){
    case SYS_EVENT_BUTTON_MENU:
      switch(sys_cur_state){
        case SYS_STATE_INIT:
        case SYS_STATE_RUN:        
        case SYS_STATE_MAX:
        case SYS_STATE_HOLD:
          sys_new_state = SYS_STATE_SET_EMS;
          break;
        case SYS_STATE_SET_EMS:
          sys_new_state = SYS_STATE_SET_HI;
          break;
        case SYS_STATE_SET_HI:
          sys_new_state = SYS_STATE_SET_LO;
          break;
        case SYS_STATE_SET_LO:
          sys_new_state = SYS_STATE_DISP_SWITCH;
          break;
        case SYS_STATE_DISP_SWITCH:
          sys_new_state = SYS_STATE_SET_EMS;
          break;
        case SYS_STATE_CAL_1:
          p_cal_param->therm_basic= therm_val.therm_max;
          p_cal_param->env_tem_1 = g_temp_result.therm_temp;
          p_cal_param->env_val_1 = therm_val.env_avg;
          flash_write_cal_param();
          sys_new_state = SYS_STATE_CAL_2;
          break;
        case SYS_STATE_CAL_2:
          delta_temp = CAL_2_TEMP - g_temp_result.env_temp;// - g_temp_result.max_temp;
          delta_cal_therm = delta_temp_2_value(delta_temp);
          delta_dtc_therm = therm_val.therm_max - p_cal_param->therm_basic;
          delta_dtc_therm = delta_dtc_therm/(p_system_setting->ems/100.0);
          p_cal_param->therm_corr = delta_cal_therm * 1.0 / delta_dtc_therm;
          flash_write_cal_param();
          sys_new_state = SYS_STATE_CAL_3;
          break;
        case SYS_STATE_CAL_3:
          sys_new_state = SYS_STATE_CAL_1;
          break;
      }
      break;
    
    case SYS_EVENT_BUTTON_UP:
      switch(sys_cur_state){
        case SYS_STATE_RUN:
        case SYS_STATE_HOLD:
          p_system_setting->light_on = !p_system_setting->light_on;
          light_on(p_system_setting->light_on);
          break;
        case SYS_STATE_SET_EMS:
          if(++p_system_setting->ems > 100){
            p_system_setting->ems = 100;
          }
          break;
        case SYS_STATE_SET_HI:
          if(++p_system_setting->hi_temp > MAX_HI_TEMP){
            p_system_setting->hi_temp = MAX_HI_TEMP;
          }
          break;
        case SYS_STATE_SET_LO:
          if(++p_system_setting->lo_temp > MAX_HI_TEMP){
            p_system_setting->lo_temp = MAX_HI_TEMP;
          }
          break;
        case SYS_STATE_DISP_SWITCH:
          p_system_setting->max_min = !p_system_setting->max_min;
          break;
        case SYS_STATE_CAL_1:
        case SYS_STATE_CAL_2:
        case SYS_STATE_CAL_3:
          g_temp_result.therm_temp++;
          break;
        case SYS_STATE_MAX:
          break;
      }
      sys_settings_changed = 1;
      break;

    case SYS_EVENT_BUTTON_DOWN:
      switch(sys_cur_state){
        case SYS_STATE_RUN:
        case SYS_STATE_HOLD:
        case SYS_STATE_MAX:
          p_system_setting->laser_on= !p_system_setting->laser_on;
          laser_on(p_system_setting->laser_on);
          break;
	      case SYS_STATE_SET_EMS:
          if(--p_system_setting->ems == 0){
            p_system_setting->ems = 1;
          }
          break;
        case SYS_STATE_SET_HI:
          if(--p_system_setting->hi_temp < MIN_LO_TEMP){
            p_system_setting->hi_temp = MIN_LO_TEMP;
          }
          break;
        case SYS_STATE_SET_LO:
          if(--p_system_setting->lo_temp < MIN_LO_TEMP){
            p_system_setting->lo_temp = MIN_LO_TEMP;
          }
          break;
        case SYS_STATE_DISP_SWITCH:
          p_system_setting->c_f= !p_system_setting->c_f;
          if(p_system_setting->c_f){
            C_2_F(&(p_system_setting->hi_temp));
            C_2_F(&(p_system_setting->lo_temp));
          }else{
            F_2_C(&(p_system_setting->hi_temp));
            F_2_C(&(p_system_setting->lo_temp));
          }          
          break;
        case SYS_STATE_CAL_1:
        case SYS_STATE_CAL_2:
        case SYS_STATE_CAL_3:
          g_temp_result.therm_temp--;
          break;

      }
      sys_settings_changed = 1;
      break;
    default:
      break;
  }

//  if(sys_settings_changed == 1){
//    flash_write_system_setting();
//    sys_settings_changed = 0;
//  }

  flush_LCD_disp();
}
