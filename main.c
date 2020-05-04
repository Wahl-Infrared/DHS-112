#include "driver.h"
#include "display.h"
#include "common.h"
#include "settings.h"
#include "button.h"
#include "cal.h"

#define TEST
#ifdef TEST
#include "test.h"
#endif

/* 系统时钟设定
 * ACLK �?2768�?外接晶振
 * MCLK 1M Hz
*/
void CLK_Init()
{

  SCFQCTL = SCFQ_1M;                      // MCLK 设置�?1M
  FLL_CTL0 |= XCAP10PF;                     // 10pF
 // FLL_CTL1 = 0x00;                        // SMCLK = LF
}

int main( void )
{
//  unsigned int hold_count;
  driver_init();
  button_init();
  settings_init();
  cal_init();

  
  /*初始�?AD 转换*/
//  therm_init();
  //SD16_init();

  /*读取flash*/  
  //flash_init();
 // if(flash_read_system_setting() != 0){
 //     set_default_system_settings();
 // }
//  if(flash_read_cal_param() != 0){
//      set_default_cal_param();
//  }

	__enable_interrupt();
	


//  thermistor_test();
//  temp_env_test();

//cal_process();
  env_test_process();

  if(cal_check() == 1){
    cal_process();
  }else{
    sys_process();
  }
}
