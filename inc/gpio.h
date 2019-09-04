#ifndef __GPIO_H__
#define __GPIO_H__

#include "io430.h"

#define LASER_CTL P1OUT_bit.P0
#define LIGHT_CTL P1OUT_bit.P1
#define BELL_CTL P2OUT_bit.P2

enum{
  BUTTON_MENU,
  BUTTON_MENU_SHORT,
  BUTTON_MENU_1_S,
  BUTTON_MENU_5_S,
  BUTTON_MENU_10_S,

  BUTTON_UP,
  BUTTON_UP_SHORT,
  BUTTON_UP_1_S,
  BUTTON_UP_5_S,
  BUTTON_UP_10_S,

  BUTTON_DOWN,
  BUTTON_DOWN_SHORT,
  BUTTON_DOWN_1_S,
  BUTTON_DOWN_5_S,
  BUTTON_DOWN_10_S,

  BUTTON_NONE
};

extern unsigned char button;

void gpio_init(void);
#endif