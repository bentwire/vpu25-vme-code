#include <stdint.h>

#include "IO.h"
#include "mc68153.h"
#include "mc68230.h"
#include "ascu2.h"

// Default to 2x what I have in my chassis.
#ifndef ASCU2_MAX_DEV
#define ASCU2_MAX_DEV 2
#endif

static const uint16_t ascu2_mpcc_offs = 0x0000; // MPCC Offset
static const uint16_t ascu2_pit1_offs = 0x0040; // PIT 1 Offset
static const uint16_t ascu2_pit2_offs = 0x0080; // PIT 2 Offset
static const uint16_t ascu2_rtc_offs  = 0x00C0; // RTC Offset
static const uint16_t ascu2_bim1_offs = 0x0100; // BIM 1 Offset
static const uint16_t ascu2_bim2_offs = 0x0110; // BIM 2 Offset
static const uint16_t ascu2_bim3_offs = 0x0120; // BIM 3 Offset
static const uint16_t ascu2_bim4_offs = 0x0130; // BIM 4 Offset
static const uint16_t ascu2_gpib_offs = 0x0140; // GPIB Offset

struct _ascu2_dev_t
{
    uint8_t * base;
    mc68230_dev_t * pit1;
    mc68230_dev_t * pit2;
    mc68153_dev_t * bim1;
    mc68153_dev_t * bim2;
    mc68153_dev_t * bim3;
    mc68153_dev_t * bim4;
};

static ascu2_dev_t DEVS[ASCU2_MAX_DEV];  //  There really should only be 1....
static uint32_t devs = 0;

/**
 * Initialize the basic functions on the ASCU2.
 *
 * This initializes both PITs and sets up the default VME bus error timeout as well as the bus clear timer.
 */
ascu2_dev_t * ASCU2Init(uint8_t * const base)
{
    if (devs >= ASCU2_MAX_DEV) return NULL;

    DEVS[devs].base = base;
    DEVS[devs].pit1 = MC68230Init(base + ascu2_pit1_offs); //(mc68230_dev_t){ .addr = base + ascu2_pit1_offs, }; // PIT1 Bus/IACK timer, PMM/ACFAIL/SYSRESET, interval timer.
    DEVS[devs].pit2 = MC68230Init(base + ascu2_pit2_offs); //(mc68230_dev_t){ .addr = base + ascu2_pit2_offs, }; // PIT2 Prg Int 9-16, BCLR mode select, BCLR Timer, GPIB Address Jumpers, GPIB Dir/Status T/L Switch.
    DEVS[devs].bim1 = MC68153Init(base + ascu2_bim1_offs);
    DEVS[devs].bim2 = MC68153Init(base + ascu2_bim2_offs);
    DEVS[devs].bim3 = MC68153Init(base + ascu2_bim3_offs);
    DEVS[devs].bim4 = MC68153Init(base + ascu2_bim4_offs);

    MC68230SetPGCR(DEVS[devs].pit1, MC68230GetPGCR(DEVS[devs].pit1) & 0x3F); // Clear bits 7 and 6.
    MC68230SetPACR(DEVS[devs].pit1, MC68230GetPACR(DEVS[devs].pit1) | 0x80); // Set PI/T 1 to bit io mode.
    MC68230SetPBCR(DEVS[devs].pit1, MC68230GetPBCR(DEVS[devs].pit1) | 0x40); // Set PI/T 1 to bit io mode.
    MC68230SetPBDR(DEVS[devs].pit1, 0xEE);  // Set Bus Timer to 32us Data and IACX timeout
    MC68230SetPBDDR(DEVS[devs].pit1, 0xDF); // Set Bus Timer to 32us Data and IACX timeout

    MC68230SetCPR(DEVS[devs].pit2, 0x1000); // Set watchdgog counter for BCLR* timer
    MC68230SetPCDR(DEVS[devs].pit2, 0xFD);  // BCLR* Mode Selection
    MC68230SetPCDDR(DEVS[devs].pit2, 0x5B); // BCLR* Mode Selection

    return &DEVS[devs++];
}

/**
 * Configure nSYSFAIL interrupt.
 */
void ASCU2ConfigureSysFailInt(ascu2_dev_t * dev, uint8_t id, uint8_t lvl)
{
    if (lvl > 7) lvl = 7;

    MC68153ConfigureInt(dev->bim1, MC68153_INT_3, false, false, false, id, lvl);
}

/**
 * Enable nSYSFAIL interrupt.
 */
void ASCU2EnableSysFailInt(ascu2_dev_t * dev)
{
    MC68153EnableInt(dev->bim1, MC68153_INT_3, false);
}

/**
 * Disable nSYSFAIL interrupt.
 */
void ASCU2DisableSysFailInt(ascu2_dev_t * dev)
{
    MC68153DisableInt(dev->bim1, MC68153_INT_3);
}

/**
 * Configure nACFAIL interrupt.
 */
void ASCU2ConfigureACFailInt(ascu2_dev_t * dev, uint8_t id, uint8_t lvl)
{
    if (lvl > 7) lvl = 7;

    MC68153ConfigureInt(dev->bim1, MC68153_INT_2, false, false, false, id, lvl);
}

/**
 * Enable nACFAIL interrupt.
 */
void ASCU2EnableACFailInt(ascu2_dev_t * dev)
{
    MC68153EnableInt(dev->bim1, MC68153_INT_2, false);
}

/**
 * Disable nACFAIL interrupt.
 */
void ASCU2DisableACFailInt(ascu2_dev_t * dev)
{
    MC68153DisableInt(dev->bim1, MC68153_INT_2);
}

/**
 * Configure TEST interrupt.
 */
void ASCU2ConfigureTESTInt(ascu2_dev_t * dev, uint8_t id, uint8_t lvl)
{
    if (lvl > 7) lvl = 7;

    MC68153ConfigureInt(dev->bim2, MC68153_INT_0, false, false, false, id, lvl);
}

/**
 * Enable TEST interrupt.
 */
void ASCU2EnableTESTInt(ascu2_dev_t * dev)
{
    MC68153EnableInt(dev->bim2, MC68153_INT_0, false);
}

/**
 * Disable TEST interrupt.
 */
void ASCU2DisableTESTInt(ascu2_dev_t * dev)
{
    MC68153DisableInt(dev->bim2, MC68153_INT_0);
}

/**
 * Set bus clear timer.
 */
void ASCU2SetBClrTimeout(ascu2_dev_t * dev, uint32_t timeout)
{
}

/**
 * Set the interrupt timer.
 */
void ASCU2SetTimer(ascu2_dev_t * dev, uint32_t timeout)
{
}
/* vim: set ai expandtab ts=4 sts=4 sw=4: */
