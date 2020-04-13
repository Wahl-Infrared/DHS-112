#ifndef __SYSTEM_H__
#define __SYSTEM_H__
#include "common.h"

enum SYS_STATE{
  SYS_STATE_INIT,
  SYS_STATE_DISP_THERMOPILE,
  SYS_STATE_DISP_THERMISTOR,
  SYS_STATE_DISP_ENV,
  SYS_STATE_RUN,
  SYS_STATE_HOLD,
  SYS_STATE_SET_PROCESS,
  SYS_STATE_MAX
};

void set_sys_state(u8 state);
u8 get_sys_state();

#endif
