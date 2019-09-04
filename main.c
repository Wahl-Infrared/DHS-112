#include "io430.h"
#include "lcd.h"
#include "uart.h"
#include "therm_sensor.h"
#include "gpio.h"
#include "system.h"
#include "stdio.h"
#include "flash.h"

void CLK_Init()
{
  WDTCTL = WDTPW +WDTHOLD;                  // Stop Watchdog Timer
//  SCFQCTL = SCFQ_1M;                      // 1M
  FLL_CTL0 |= XCAP10PF;                     // XT2 off
//  FLL_CTL1 = 0x00;                        // SMCLK = LF
}

int main( void )
{
  unsigned int hold_count;

  CLK_Init();

  /*初始化GPIO--PORT1*/
  gpio_init();
  
  /*初始化 AD 转换*/
//  therm_init();
  AD_init();

  /*读取flash*/  
  flash_init();
  if(flash_read_system_setting() != 0){
      set_default_system_settings();
  }
  if(flash_read_cal_param() != 0){
      set_default_cal_param();
  }

  /*初始化 LCD*/
  LCD_Init();


  /*打开中断总开关*/
  __enable_interrupt();
  sys_cur_state = SYS_STATE_INIT;

  
  P2OUT_bit.P4 = 0;
  if(P2IN_bit.P5 == 0){
    P2OUT_bit.P4 = 1;
    if(P2IN_bit.P5 == 1){
      sys_new_state = SYS_STATE_CAL_1;
      flush_LCD_disp();
      set_default_cal_param();
      cal_process();
    }
  }

   /*初始化 UART*/
  UART_Init(4800,'n',8,1);
  
  while(1){
    if(P1IN_bit.P7 == 0){
 //     if(sys_cur_state != SYS_STATE_RUN){
 //       sys_new_state = SYS_STATE_RUN;
      if(sys_cur_state != SYS_STATE_READ_ENV_TEMP){
        sys_new_state = SYS_STATE_READ_ENV_TEMP; 
        g_temp_result.max_temp = MIN_TEMP;
        g_temp_result.min_temp = MAX_TEMP;
        hold_count = 0;
        flush_LCD_disp();
      }
      read_therm_temp();
    }else{
      if(sys_cur_state == SYS_STATE_RUN){
        sys_new_state = SYS_STATE_HOLD;
        flush_LCD_disp();
      }else if(sys_cur_state == SYS_STATE_HOLD){
        __delay_cycles(10000);
        if(hold_count++ == 8000){
          if(sys_settings_changed){
            flash_write_system_setting();
          }
          P2OUT_bit.P0 = 0;
        }
      }
    }
  }
}
