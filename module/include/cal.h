#ifndef _CAL_H_
#define  _CAL_H_

typedef struct _CALIBRATION_{
  float R_thermistor_plug_up;
  float gain;
  u16 thermopile_val_env;
}CALIBRATION, *P_CALIBRATION;

extern P_CALIBRATION p_calibration;

void cal_process(void);
void cal_init();
u8 cal_check();





#endif
