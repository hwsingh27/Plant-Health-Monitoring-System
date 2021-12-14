#include "common.h"

void sysclock_init()
{
  // Corresponds to FEI mode as shown in sec 24.4.1

  // Select PLL/FLL as clock source
  MCG->C1 &= ~(MCG_C1_CLKS_MASK);
  MCG->C1 |= MCG_C1_CLKS(0);

  // Use internal reference clock as source for the FLL
  MCG->C1 |= MCG_C1_IREFS(1);

  // Select the FLL (by setting "PLL select" to 0)
  MCG->C6 &= ~(MCG_C6_PLLS_MASK);
  MCG->C6 |= MCG_C6_PLLS(0);

  // Select 24 MHz - see table for MCG_C4[DMX32]
  MCG->C4 &= ~(MCG_C4_DRST_DRS_MASK & MCG_C4_DMX32_MASK);
  MCG->C4 |= MCG_C4_DRST_DRS(0);
  MCG->C4 |= MCG_C4_DMX32(1);
}
