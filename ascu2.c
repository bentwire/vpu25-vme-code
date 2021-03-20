#include <stdint.h>

#include "IO.h"
#include "mc68230.h"
#include "ascu2.h"

uint8_t * const ascu2_addr     = &__vme16_start + 0x0000;
const uint16_t ascu2_mpcc_offs = 0x0000; // MPCC Offset
const uint16_t ascu2_pit1_offs = 0x0040; // PIT 1 Offset
const uint16_t ascu2_pit2_offs = 0x0080; // PIT 2 Offset
const uint16_t ascu2_rtc_offs  = 0x00C0; // RTC Offset
const uint16_t ascu2_bim1_offs = 0x0100; // BIM 1 Offset
const uint16_t ascu2_bim2_offs = 0x0110; // BIM 2 Offset
const uint16_t ascu2_bim3_offs = 0x0120; // BIM 3 Offset
const uint16_t ascu2_bim4_offs = 0x0130; // BIM 4 Offset
const uint16_t ascu2_gpib_offs = 0x0140; // GPIB Offset

static mc68230_dev_t pit1 = { .addr = ascu2_addr + ascu2_pit1_offs, }; // PIT1 Bus/IACK timer, PMM/ACFAIL/SYSRESET, interval timer.
static mc68230_dev_t pit2 = { .addr = ascu2_addr + ascu2_pit2_offs, }; // PIT2 Prg Int 9-16, BCLR mode select, BCLR Timer, GPIB Address Jumpers, GPIB Dir/Status T/L Switch.

/**
 * Initialize the basic functions on the ASCU2.
 *
 * This initializes both PITs and sets up the default VME bus error timeout as well as the bus clear timer.
 */
void ASCU2Init(void)
{
}

/**
 * Set bus clear timer.
 */
void ASCU2SetBClrTimeout(uint32_t timeout)
{
}

/**
 * Set the interrupt timer.
 */
void ASCU2SetTimer(uint32_t timeout)
{
}
