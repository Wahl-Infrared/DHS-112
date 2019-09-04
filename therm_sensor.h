#ifndef __AD_CONVERT__
#define __AD_CONVERT__

#include "stdbool.h"

#define CURVE_ARRAY_SIZE 32
#define TEMP_ARRAY_SIZE 8
#define MAX_HI_TEMP 3000
#define MIN_LO_TEMP -500
#define CAL_2_TEMP 1000    /*100 ��*/
#define MAX_TEMP 3000
#define MIN_TEMP -500

typedef struct _ST_TEMP_SENSOR_{
  unsigned int therm_value[TEMP_ARRAY_SIZE];
  unsigned int env_value[TEMP_ARRAY_SIZE];
  unsigned char therm_array_offset;
  unsigned char env_array_offset;
  unsigned char therm_value_flag:1;
  unsigned char env_value_flag:1;
}ST_TEMP_SENSOR;

typedef struct _ST_TEMP_VAL_{
  unsigned int therm_value[TEMP_ARRAY_SIZE];
  unsigned int env_value[TEMP_ARRAY_SIZE];
  unsigned int therm_avg;
  unsigned int env_avg;
  unsigned int therm_max;
  unsigned int env_max;
  unsigned char value_offset;
}ST_TEMP_VAL;

typedef struct _ST_TEMP_RESULT_{
  int therm_temp;
  int env_temp;
  int max_temp;
  int min_temp;
}ST_TEMP_RESULT,*PST_TEMP_RESULT;

typedef struct _CAL_PARAM_{
  unsigned int R_env_pullup;
  unsigned int env_val_1;
  unsigned int env_val_2;
  unsigned int therm_basic;
  int env_tem_1;
  int env_tem_2;
  float therm_corr;
}CAL_PARAM, *PCAL_PARAM;

typedef struct _TEMP_CURVE_{
  unsigned int delta_temp;
  unsigned int delta_therm_value;
}TEMP_CURVE, *PTEMP_CURVE;

extern ST_TEMP_RESULT g_temp_result;
extern ST_TEMP_VAL therm_val;
extern ST_TEMP_SENSOR therm_sensor;    /*  should be delete in stable version*/
extern PCAL_PARAM p_cal_param;
extern const TEMP_CURVE curve[CURVE_ARRAY_SIZE];

void set_default_cal_param(void);
void calc_temp_process(void);
void AD_init(void);
void temp_detect_start(bool on);
void read_therm_temp(void);
void cal_process(void);

#endif