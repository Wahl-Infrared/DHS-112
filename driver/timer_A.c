#include "io430.h"

void (*call_back_func)(unsigned char) = 0;

void timer_A_init()
{
	TACTL |= TACLR; /* 清除TAR */
  TACTL = TASSEL_1 + ID_3; /* ACLK 8分频,  使能中断*/
  TACCTL0 = CCIE;
}

void start_timer_A(unsigned int msec)
{
  TACCR0 = msec << 1; /* 8分频后，2个计数代�?1 ms */
  TACTL |= MC_1;
}

void stop_timer_A()
{
	TACTL_bit.MC0 = 0;
	TACTL_bit.MC1 = 0;
	TACCR0 = 0;
}

void register_timer_A(void (*ptr)(unsigned char))
{
  call_back_func = ptr;
}

#pragma vector = TIMERA0_VECTOR
__interrupt void timer_A_ISR(void)
{
  if(call_back_func){
    call_back_func(P1IN);
  }
}

