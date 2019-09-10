#ifndef __AD_CONVERT__
#define __AD_CONVERT__

#include "stdbool.h"

#define THERMOPILE_SUM_COUNT 64
#define THERMISTOR_SUM_COUNT 32

#define MAX_HI_TEMP 3000
#define MIN_LO_TEMP -500
#define CAL_2_TEMP 1000    /*100 ¡æ*/
#define MAX_TEMP 3000
#define MIN_TEMP -500

typedef struct _ST_THERM_SENSOR_{
  unsigned long thermopile_sum_value;
  unsigned long thermistor_sum_value;
  unsigned char thermopile_value_count;
  unsigned char thermistor_value_count;
}ST_THERM_SENSOR;

typedef struct _ST_TEMP_RESULT_{
  int therm_temp;
  int env_temp;
  int max_temp;
  int min_temp;
  int thermopile;
  int thermistor;
  int max_thermopile;
}ST_TEMP_RESULT,*PST_TEMP_RESULT;

typedef struct _CAL_PARAM_{
  float R_env_pullup;
  float gain;
  unsigned int basic_SD16_val;
}CAL_PARAM, *PCAL_PARAM;

typedef struct _TEMP_CURVE_{
  unsigned int delta_temp;
  unsigned int delta_therm_value;
}TEMP_CURVE, *PTEMP_CURVE;

extern ST_TEMP_RESULT g_temp_result;
extern ST_THERM_SENSOR therm_sensor;    /*  should be delete in stable version*/
extern PCAL_PARAM p_cal_param;

void set_default_cal_param(void);
void SD16_init(void);
unsigned int read_thermopile_value();
unsigned int read_thermistor_value();
int get_object_temp();
void cal_process(void);

#endif