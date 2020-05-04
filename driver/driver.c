#include "basic_timer.h"
#include "flash.h"
#include "gpio.h"
#include "lcd.h"
#include "sd16.h"
#include "timer_A.h"
#include "uart.h"

void driver_init()
{
  wdt_init();
  clk_init();
  bt_init();
  gpio_init();
  timer_A_init(); 
  LCD_Init();
  SD16_init();
  flash_init();
}
