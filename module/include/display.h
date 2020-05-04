#ifndef __DISPLAY_H_
#define __DISPLAY_H_

#include "lcd.h"
#include "common.h"

void display_main_temp(int temp);
void display_slave_temp(int temp);
void display_main_int(int temp);
void display_slave_int(int temp);
void display_EMS(u8 val);
void display_run_page(u8 ems, u8 cf, u8 min);
void display_cal_run_page(u8 ems, u8 cf, u8 min);
void display_set_ems(u8 ems);
void display_set_hi(int temp);
void display_set_lo(int temp);
void display_set_cf_min(u8 cf, u8 min);
void display_cf_flag(u8 cf);
void display_min_flag(u8 min);
void display_hold_flag(void);
void clear_hold_flag(void);



#endif
