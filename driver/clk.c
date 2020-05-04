#include "io430.h"

void clk_init()
{

  SCFQCTL = SCFQ_1M;                      // MCLK 设置�?1M
  FLL_CTL0 |= XCAP10PF;                     // 10pF
 // FLL_CTL1 = 0x00;                        // SMCLK = LF
}

