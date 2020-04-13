#include "io430.h"
#include "gpio.h"
#include "common.h"

#define LASER_CTL P1OUT_bit.P0
#define LIGHT_CTL P1OUT_bit.P1
#define BELL_CTL P2OUT_bit.P2

/*****************************************************
* �?   称：gpio_init()
* �?   能：初始化GPIO
* 入口参数：无
* 出口参数：无
* �?   �? P1.0 -> 激�?*           P1.1 -> 液晶背光�?*           P1.2 -> 按键1 �?*           P1.3 -> 按键2 �?*                   同时按下表示 menu �?*           P2.0 电源控制
*           P2.7 主开关检�?******************************************************/
void gpio_init(void)
{
  P1DIR = 0x03;
  P1OUT = 0x00;
  P1IES = 0x8C;
  P1IE  = 0x8C;
  P2DIR = 0x11;
  P2OUT = 0x11;
  P2SEL = 0x00;
}

void (*P1_ISR_call_back)(unsigned char);

/****************************************************************************
* �?   称：__PORT1_key()
* �?   能：PORT1 中断入口函数，按键控制�?* 入口参数：无
* 出口参数：无
* �?   �? 使用 P1.2 P1.3 作为按键读取，两个同时为低时表示 MENU按下
*           低电平表示按�?****************************************************************************/
#pragma vector = PORT1_VECTOR
__interrupt void P1_ISR(void)
{
  if(P1_ISR_call_back){
    P1_ISR_call_back(P1IN);
  }
  P1IFG = 0x00;  /* must be clear by software */
}

void register_P1_ISR(void (*ptr)(unsigned char))
{
  P1_ISR_call_back = ptr;
}
