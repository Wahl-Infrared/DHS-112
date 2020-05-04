#include "io430.h"
#include "lcd.h"

struct time_val{
	unsigned long tv_sec;
	unsigned long tv_msec;
};

struct time_val system_tv;

/*
 * Basic Timer 初始�? * fLCD �?4 MUX 模式
 * 故选择 ACLK�?2768Hz�?4分频 
 * BTFRFQx = 01, fLCD = 512Hz
 *
 * 开启中断， 1s进入一次，提供1s时钟，用于计时功�?*/
void bt_init()
{
  BTCTL = BT_fLCD_DIV64 + BT_ADLY_1000;
  IE2 |= BTIE;                                  //开启basic timer 中断
}

/****************************************************************************
*	函数名称：basic_timer_ISR()
* 功     能：basic timer 1s 触发一次， 提供系统时间 1s 基本计时， 设置LCD闪烁标志
* 参     数：无
* 返 回 值：无
****************************************************************************/
#pragma vector = BASICTIMER_VECTOR
__interrupt void basic_timer_ISR(void)
{
	system_tv.tv_sec++;
	LCD_blink();//应该替换为闪烁标志
}

/****************************************************************************
*	函数名称：get_system_time_sec()
* 功     能：获取系统时间
* 参     数：无
* 返 回 值：系统时间， 秒值
****************************************************************************/
unsigned long get_system_time_sec()
{
	return system_tv.tv_sec;
}
