#ifndef __SYSTEM_H__
#define __SYSTEM_H__

enum{
  SYS_STATE_INIT,
  SYS_STATE_DISP_THERMOPILE,
  SYS_STATE_DISP_THERMISTOR,
  SYS_STATE_DISP_ENV,
  SYS_STATE_RUN,
  SYS_STATE_HOLD,
  SYS_STATE_SET_EMS,
  SYS_STATE_SET_HI,
  SYS_STATE_SET_LO,
  SYS_STATE_DISP_SWITCH,
  SYS_STATE_CAL_1,           /*环温标定*/
  SYS_STATE_CAL_2,           /*100度黑体标定*/
  SYS_STATE_CAL_3,           /*0度环温标定*/
  SYS_STATE_READ_MEM,
  SYS_STATE_MAX
};

enum{
  SYS_EVENT_BUTTON_MENU,
  SYS_EVENT_BUTTON_UP,
  SYS_EVENT_BUTTON_DOWN,
  SYS_EVENT_BUTTON_HOLD
};

typedef struct _SYSTEM_SETTING_{
  int hi_temp;
  int lo_temp;  
  unsigned char ems;
  unsigned char max_min  :1;
  unsigned char c_f      :1;
  unsigned char laser_on :1;
  unsigned char light_on :1;
  unsigned char temp_detect_on  :1;
}SYSTEM_SETTING, *PSYSTEM_SETTING;

extern PSYSTEM_SETTING p_system_setting;
extern unsigned char sys_cur_state;
extern unsigned char sys_new_state;
extern unsigned char sys_settings_changed;

void set_default_system_settings(void);
void C_2_F(int *val);
void F_2_C(int *val);
void flush_LCD_disp(void);
void sys_event_handler(unsigned char event);

#endif