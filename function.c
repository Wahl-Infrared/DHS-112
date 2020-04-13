#include "lcd.h"
#include "gpio.h"

/*开关蜂鸣器*/
void bell_on(bool on)
{
  BELL_CTL = on;
}


/*开关激光器*/
void laser_on(bool on)
{
  LASER_CTL = on;
  LCD_flag_display(LCD_FLAG_LASER, on);
}

/*开关LCD 背景灯 */
void light_on(bool on)
{
  LIGHT_CTL = on;
  LCD_flag_display(LCD_FLAG_SUN, on);
}

