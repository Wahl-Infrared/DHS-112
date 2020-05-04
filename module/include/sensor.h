#ifndef _SENSOR_H_
#define _SENSOR_H_

#include "common.h"

float calc_evn_temp(u16 val);
float calc_R_p(u16 sd_val, u16 set_val);
float calc_gain(u16 *sd_val, u16 set_val);
float calc_temperature(u16 *val);

#endif
