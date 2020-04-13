#include "io430.h"
#include "display.h"
#include "gpio.h"
#include "sd16.h"
#include "basic_timer.h"
#include "common.h"
#include "settings.h"
#include "button.h"


/*
#include "uart.h"
#include "therm_sensor.h"
#include "gpio.h"
#include "system.h"
#include "stdio.h"
#include "flash.h"
*/

#define CONFIG_TEST
#ifdef CONFIG_TEST
#include "test/button_test.c"
#include "test/BT_test.c"
#include "test/sd16_test.c"

#endif

void WDT_Init()
{
  WDTCTL = WDTPW +WDTHOLD;                  // Stop Watchdog Timer
}

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
  WDT_Init();
  CLK_Init();
  BT_Init();
  gpio_init();
  timer_A_init(); 
  LCD_Init();
  SD16_init();
  settings_init();
  
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
	
  button_init();

while(1){
  sys_process();
  }
}
