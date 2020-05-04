#include "io430.h"

void wdt_init()
{
  WDTCTL = WDTPW +WDTHOLD;                  // Stop Watchdog Timer
}

