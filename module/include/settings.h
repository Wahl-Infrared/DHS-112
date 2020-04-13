#ifndef _SETTINGS_H_
#define _SETTINGS_H_

#include "common.h"

#define HI_TEMP_LIMIT 1500
#define LO_TEMP_LIMIT -50
#define HI_EMS_LIMIT 100
#define LO_EMS_LIMIT 0

enum{
  STEP_INIT,
  STEP_SET_EMS,
  STEP_SET_HI,
  STEP_SET_LO,
  STEP_SET_CF_MIN,
  STEP_MAX
};

typedef struct _SETTING_{
  int hi_temp;
  int lo_temp;
  u8 ems;
  union{
    u8 flags;
    struct{
      u8 min      :1;
      u8 cf       :1;
      u8 laser    :1;
      u8 light    :1;
     }flags_bit;
  };
}SETTING, *PSETTING;


extern PSETTING p_setting;

void settings_init();
void save_settings();



#endif
