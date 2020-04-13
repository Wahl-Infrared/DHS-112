#include "lcd.h"
#include "common.h"

void display_main_temp(int temp)
{
  if(temp < -99 || temp > 1000){
    LCD_main_display(temp/10,0);
  }else{
    LCD_main_display(temp,1);
  }
}

void display_slave_temp(int temp)
{
  if(temp < -99 || temp > 1000){
    LCD_slave_display(temp/10,0);
  }else{
    LCD_slave_display(temp,1);
  }
}

void display_main_int(int temp)
{
    LCD_main_display(temp,0);
}

void display_slave_int(int temp)
{
    LCD_slave_display(temp,0);
}

void display_avg_temp(int temp)
{
  if(temp < -99 || temp > 1000){
    LCD_slave_display(temp/10,0);
  }else{
    LCD_slave_display(temp,1);
  }
  LCD_flag_display(LCD_FLAG_AVG, true);
}

void display_EMS(u8 ems)
{
	LCD_EMS_display(ems);
}

void display_run_page(u8 ems, bool cf, bool min)
{
  LCD_Clear();
  LCD_flag_display(LCD_FLAG_EMS_FLAG, 1);
  LCD_flag_display(LCD_FLAG_EMS_DOT, 1);
  LCD_flag_display(LCD_FLAG_EMS_EQU, 1);
  LCD_EMS_display(ems);
  if(cf){
    LCD_flag_display(LCD_FLAG_TEMP_F, 1);
  }else{
    LCD_flag_display(LCD_FLAG_TEMP_C, 1);
  }
  if(min){
    LCD_flag_display(LCD_FLAG_MIN, 1);
  }else{
    LCD_flag_display(LCD_FLAG_MAX, 1);
  }
}

void display_set_ems(u8 ems)
{
  LCD_Clear();
  LCD_main_display(1, 0);
  LCD_flag_display(LCD_FLAG_SET, 1);
  LCD_blink_display(LCD_FLAG_EMS_FLAG, 1);
  LCD_flag_display(LCD_FLAG_EMS_EQU, 1);
  LCD_EMS_display(ems);
}

void display_set_hi(int temp)
{
  LCD_Clear();
  LCD_main_display(2, 0);
  LCD_flag_display(LCD_FLAG_SET, 1);
  LCD_blink_display(LCD_FLAG_HI, 1);
  LCD_slave_display(temp, 0 );
}

void display_set_lo(int temp)
{
  LCD_Clear();
  LCD_main_display(3, 0);
  LCD_flag_display(LCD_FLAG_SET, 1);
  LCD_blink_display(LCD_FLAG_LO, 1);
  LCD_slave_display(temp, 0);
}

void display_set_cf_min(u8 cf, u8 min)
{
  LCD_Clear();
  LCD_main_display(4, 0);
  LCD_flag_display(LCD_FLAG_SET, 1);
  if(cf){
    LCD_blink_display(LCD_FLAG_TEMP_F, 1);
  }else{
    LCD_blink_display(LCD_FLAG_TEMP_C, 1);
  }
  if(min){
    LCD_blink_display(LCD_FLAG_MIN, 1);
  }else{
    LCD_blink_display(LCD_FLAG_MAX, 1);
  }
}


void display_cf_flag(u8 cf)
{
  if(cf){
    LCD_flag_display(LCD_FLAG_TEMP_C, 0);
    LCD_flag_display(LCD_FLAG_TEMP_F, 1);
  }else{
    LCD_flag_display(LCD_FLAG_TEMP_C, 1);
    LCD_flag_display(LCD_FLAG_TEMP_F, 0);
  }
}

void display_min_flag(u8 min)
{
  if(min){
    LCD_flag_display(LCD_FLAG_MAX, 0);
    LCD_flag_display(LCD_FLAG_MIN, 1);
  }else{
    LCD_flag_display(LCD_FLAG_MAX, 1);
    LCD_flag_display(LCD_FLAG_MIN, 0);
  }
}


void display_hold_flag()
{
  LCD_flag_display(LCD_FLAG_HOLD, 1);
}
