#include "io430.h"
#include "system.h"
#include "gpio.h"

#define BUTTON_MAX_COUNT 1500
#define BUTTON_SHORT_COUNT 20
#define BUTTON_1S_COUNT 100
#define BUTTON_5S_COUNT 500
#define BUTTON_10S_COUNT 1000
#define BUTTON_EVENT_FREQ_1 100
#define BUTTON_EVENT_FREQ_3 20
#define BUTTON_EVENT_FREQ_10 5

unsigned char button;

void timer_A_init()
{
  TACTL = TASSEL_1 + ID_0 + TACLR; /* ACLK 不分频, 清除TAR， 使能中断*/
  TACCTL0 = CCIE;
  TACCR0 = 327; /* 10ms 延时消抖*/
}

void timer_A_stop()
{
  TACTL_bit.MC0 = 0;
  TACTL_bit.MC1 = 0;
  TACTL |= TACLR;
}

/*****************************************************
* 名    称：gpio_init()
* 功    能：初始化GPIO
* 入口参数：无
* 出口参数：无
* 说    明: P1.0 -> 激光
*           P1.1 -> 液晶背光灯
*           P1.2 -> 按键1 下
*           P1.3 -> 按键2 上
*                   同时按下表示 menu 键
*           P2.0 电源控制
*           P2.7 主开关检测
******************************************************/
void gpio_init(void)
{
  timer_A_init();
  P1DIR = 0x03;
  P1OUT = 0x00;
  P1IES = 0x0C;
  P1IE = 0x0C;
  P2DIR = 0x11;
  P2OUT = 0x11;
  P2SEL = 0x00;    

  /*
  P2SEL_bit.P4 = 0;
  P2SEL_bit.P5 = 0;
  P2DIR_bit.P4 = 1;
  P2DIR_bit.P5 = 1;
  P2OUT_bit.P4 = 1;
  P2OUT_bit.P5 = 1;
  P2IES_bit.P4 = 0;
  P2IES_bit.P5 = 0;
  P2IE_bit.P4 = 0;
  P2IE_bit.P5 = 0;

  
  P1DIR |= BIT0;
  P1DIR |= BIT1;
  P1DIR &= ~BIT2;
  P1DIR &= ~BIT3;
  P1OUT |= BIT0;   //LASER
  P1OUT |= BIT1;    // 液晶背板
  P1IES |= BIT2;    //按键
  P1IES |= BIT3;     //按键
  P1IE |= BIT2;
  P1IE |= BIT3;
  
  P1OUT=0x00;
*/
}

/****************************************************************************
* 名    称：__PORT1_key()
* 功    能：PORT1 中断入口函数，按键控制。
* 入口参数：无
* 出口参数：无
* 说    明: 使用 P1.2 P1.3 作为按键读取，两个同时为低时表示 MENU按下
*           低电平表示按下
****************************************************************************/
#pragma vector = PORT1_VECTOR
__interrupt void button_ISR(void)
{
  if(P1IN_bit.P2 == 0 && P1IN_bit.P3 == 0){
    button = BUTTON_MENU;
  }else if(P1IN_bit.P2 == 0){
     button = BUTTON_UP;
  }else if(P1IN_bit.P3 == 0){
     button = BUTTON_DOWN;
  }else{
    button= BUTTON_NONE;
  }
 // LCD_main_display(test, val);
//  LCD_blink_display(test, 1);
//  LCD_main_display(test, 0);

  /*
  if(key_flag){
    key_flag = 0;
    LCD_flag_display(LCD_FLAG_LOCK,1);
  }else{
    key_flag = 1;
    LCD_flag_display(LCD_FLAG_LOCK,0);
  }
  */
  
  TACTL |= MC_1;
  P1IFG = 0x00;
}

#pragma vector = TIMERA0_VECTOR
__interrupt void timer_A_ISR(void)
{
  static unsigned int count = 0;
  
  switch(button){
    case BUTTON_MENU:
      if(P1IN_bit.P2 == 0 && P1IN_bit.P3 == 0){     /* 按下按键 */
        if(count++ > BUTTON_SHORT_COUNT){
          button = BUTTON_MENU_SHORT;
          sys_event_handler(SYS_EVENT_BUTTON_MENU);
        }
      }else{                          /* 释放按键 */
        goto BUTTON_RELEASE;
      }
      break;
    case BUTTON_MENU_SHORT:
      if(P1IN_bit.P2 == 0 && P1IN_bit.P3 == 0){     /* 按下按键 */
        if(count++ > BUTTON_1S_COUNT){
          button = BUTTON_MENU_1_S;
          sys_event_handler(SYS_EVENT_BUTTON_MENU);
        }
      }else{                        /* 释放按键 */
        goto BUTTON_RELEASE;
      }
      break;
    case BUTTON_MENU_1_S:
      if(P1IN_bit.P2 == 0 && P1IN_bit.P3 == 0){     /* 按下按键 */
        if(count++ > BUTTON_5S_COUNT){
          button = BUTTON_MENU_5_S;
          sys_event_handler(SYS_EVENT_BUTTON_MENU);
        }
      }else{                        /* 释放按键 */
        goto BUTTON_RELEASE;
      }
      
      if(count%BUTTON_EVENT_FREQ_1 == 0){
        sys_event_handler(SYS_EVENT_BUTTON_MENU);      
      }
      break;
    case BUTTON_MENU_5_S:
       if(P1IN_bit.P2 == 0 && P1IN_bit.P3 == 0){     /* 按下按键 */
        if(count++ > BUTTON_10S_COUNT){
          button = BUTTON_MENU_10_S;
          sys_event_handler(SYS_EVENT_BUTTON_MENU);
        }
      }else{                        /* 释放按键 */
        goto BUTTON_RELEASE;
      }
      if(count%BUTTON_EVENT_FREQ_3 == 0){
        sys_event_handler(SYS_EVENT_BUTTON_MENU);      
      }
      break;
    case BUTTON_MENU_10_S:
      if(P1IN_bit.P2 == 0 && P1IN_bit.P3 == 0){     /* 按下按键 */
        if(count++ > BUTTON_MAX_COUNT ){
          count = BUTTON_10S_COUNT;
        }          
      }else{                                          /* 按键释放 */
       goto BUTTON_RELEASE;
      }
      if(count%BUTTON_EVENT_FREQ_10 == 0){
        sys_event_handler(SYS_EVENT_BUTTON_MENU);      
      }
      break;

    case BUTTON_UP:
      if(P1IN_bit.P2 == 0 && P1IN_bit.P3 != 0){     /* 按下按键 */
        if(count++ > BUTTON_SHORT_COUNT){
          button = BUTTON_UP_SHORT;
          sys_event_handler(SYS_EVENT_BUTTON_UP);
        }
      }else{                          /* 释放按键 */
        goto BUTTON_RELEASE;
      }
      break;
    case BUTTON_UP_SHORT:
      if(P1IN_bit.P2 == 0 && P1IN_bit.P3 != 0){     /* 按下按键 */
        if(count++ > BUTTON_1S_COUNT){
          button = BUTTON_UP_1_S;
          sys_event_handler(SYS_EVENT_BUTTON_UP);
        }
      }else{                          /* 释放按键 */
        goto BUTTON_RELEASE;
      }
      break;
    case BUTTON_UP_1_S:
      if(P1IN_bit.P2 == 0 && P1IN_bit.P3 != 0){     /* 按下按键 */
        if(count++ > BUTTON_5S_COUNT){
          button = BUTTON_UP_5_S;
          sys_event_handler(SYS_EVENT_BUTTON_UP);
        }
      }else{                          /* 释放按键 */
        goto BUTTON_RELEASE;
      }
      if(count%BUTTON_EVENT_FREQ_1 == 0){
        sys_event_handler(SYS_EVENT_BUTTON_UP);      
      }
      break;
    case BUTTON_UP_5_S:
      if(P1IN_bit.P2 == 0 && P1IN_bit.P3 != 0){     /* 按下按键 */
        if(count++ > BUTTON_10S_COUNT){
          button = BUTTON_UP_10_S;
          sys_event_handler(SYS_EVENT_BUTTON_UP);
        }
      }else{                          /* 释放按键 */
        goto BUTTON_RELEASE;
      }
      if(count%BUTTON_EVENT_FREQ_3 == 0){
        sys_event_handler(SYS_EVENT_BUTTON_UP);      
      }
      break;
    case BUTTON_UP_10_S:
      if(P1IN_bit.P2 == 0 && P1IN_bit.P3 != 0){     /* 按下按键 */
        if(count++ > BUTTON_MAX_COUNT ){
          count = BUTTON_10S_COUNT;
        }
      }else{                          /* 释放按键 */
        goto BUTTON_RELEASE;
      }
      if(count%BUTTON_EVENT_FREQ_10 == 0){
        sys_event_handler(SYS_EVENT_BUTTON_UP);      
      }
      break;

    case BUTTON_DOWN:
      if(P1IN_bit.P2 != 0 && P1IN_bit.P3 == 0){
        if(count++ > BUTTON_SHORT_COUNT){
          button = BUTTON_DOWN_SHORT;
          sys_event_handler(SYS_EVENT_BUTTON_DOWN);
        }
      }else{
        goto BUTTON_RELEASE;
      }
      break;
    case BUTTON_DOWN_SHORT:
      if(P1IN_bit.P2 != 0 && P1IN_bit.P3 == 0){
        if(count++ > BUTTON_1S_COUNT){
          button = BUTTON_DOWN_1_S;
          sys_event_handler(SYS_EVENT_BUTTON_DOWN);
        }
      }else{
        goto BUTTON_RELEASE;
      }
      break;
    case BUTTON_DOWN_1_S:
      if(P1IN_bit.P2 != 0 && P1IN_bit.P3 == 0){
        if(count++ > BUTTON_5S_COUNT){
          button = BUTTON_DOWN_5_S;
          sys_event_handler(SYS_EVENT_BUTTON_DOWN);
        }
      }else{
        goto BUTTON_RELEASE;
      }
      if(count%BUTTON_EVENT_FREQ_1 == 0){
        sys_event_handler(SYS_EVENT_BUTTON_DOWN);      
      }
      break;
    case BUTTON_DOWN_5_S:
      if(P1IN_bit.P2 != 0 && P1IN_bit.P3 == 0){
        if(count++ > BUTTON_10S_COUNT){
          button = BUTTON_DOWN_10_S;
          sys_event_handler(SYS_EVENT_BUTTON_DOWN);
        }
      }else{
        goto BUTTON_RELEASE;
      }
      if(count%BUTTON_EVENT_FREQ_3 == 0){
        sys_event_handler(SYS_EVENT_BUTTON_DOWN);      
      }
      break;
    case BUTTON_DOWN_10_S:
      if(P1IN_bit.P2 != 0 && P1IN_bit.P3 == 0){
        if(count++ > BUTTON_MAX_COUNT ){
          count = BUTTON_10S_COUNT;
        }
      }else{
        goto BUTTON_RELEASE;
      }
      if(count%BUTTON_EVENT_FREQ_10 == 0){
        sys_event_handler(SYS_EVENT_BUTTON_DOWN);
      }
      break;
      
    default:
      goto BUTTON_RELEASE;
      break;
  }
  
  return;
  
BUTTON_RELEASE:
  button = BUTTON_NONE;
  count = 0;
  timer_A_stop();
}