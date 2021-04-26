#include <stdio.h>

#include "IO.h"
#include "mc68153.h"

struct _mc68153_dev_t
{
    uint8_t * addr;
    bool isEnabled[MC68153_INT_MAX]; // Is this channel enabled?
};

#ifndef MC68153_MAX_DEV
#define MC68153_MAX_DEV 8
#endif

static mc68153_dev_t DEVS[MC68153_MAX_DEV];
static uint32_t devs = 0;

/**
 * Get pointer to a BIM dev at address.
 *
 */
mc68153_dev_t * MC68153Init(uint8_t * const addr)
{
    if (devs >= MC68153_MAX_DEV) return NULL; // Maybe abort instead???

    mc68153_dev_t * dev = &DEVS[devs++];

    dev->addr = (uint8_t *)addr;

    return dev;
}

typedef enum
{
    FLAG       = 0x80,
    FAC        = 0x40,
    EXTERNAL   = 0x20,
    ENABLE     = 0x10,
    IRAC       = 0x08,
    LEVEL_MASK = 0x07,
} mc68153_cr_bits_t;

typedef enum
{
    CR0 = 0,
    CR1 = 1,
    CR2 = 2,
    CR3 = 3,
    VR0 = 4,
    VR1 = 5,
    VR2 = 6,
    VR3 = 7,

} mc68153_reg_off_t;

/**
 * Sets the level bits in CR(which) to lvl
 *
 */
static void MC68153SetCRLvl(mc68153_dev_t * dev, mc68153_int_t which, uint8_t lvl)
{
    uint8_t * reg;

    if (lvl > 7) lvl = 7;

    switch (which)
    {
        case MC68153_INT_0:
            reg = dev->addr + (CR0 * 2) + 1;
            break;
        case MC68153_INT_1:
            reg = dev->addr + (CR1 * 2) + 1;
            break;
        case MC68153_INT_2:
            reg = dev->addr + (CR2 * 2) + 1;
            break;
        case MC68153_INT_3:
            reg = dev->addr + (CR3 * 2) + 1;
            break;
        default:
            return; // Abort?
    }

    lvl &= LEVEL_MASK; // Make sure the LEVEL_MASK bits are the only bits we are changing.
    *(reg) &= ~LEVEL_MASK; // Clear all the lvl bits.
    *(reg) |= lvl; // set lvl bits.
}

/**
 * Sets the bits in CR(which) that are set in <bits>
 *
 */
static void MC68153SetCRBits(mc68153_dev_t * dev, mc68153_int_t which, uint8_t bits)
{
    uint8_t * reg;

    switch (which)
    {
        case MC68153_INT_0:
            reg = dev->addr + (CR0 * 2) + 1;
            break;
        case MC68153_INT_1:
            reg = dev->addr + (CR1 * 2) + 1;
            break;
        case MC68153_INT_2:
            reg = dev->addr + (CR2 * 2) + 1;
            break;
        case MC68153_INT_3:
            reg = dev->addr + (CR3 * 2) + 1;
            break;
        default:
            return; // Abort?
    }

    bits &= ~LEVEL_MASK; // Make sure the LEVEL_MASK bits are not set in the list of bits to set.

    *(reg) |= bits;
}

/**
 * Clears the bits in CR(which) that are set in <bits>
 *
 */
static void MC68153ClrCRBits(mc68153_dev_t * dev, mc68153_int_t which, uint8_t bits)
{
    uint8_t * reg;

    switch (which)
    {
        case MC68153_INT_0:
            reg = dev->addr + (CR0 * 2) + 1;
            break;
        case MC68153_INT_1:
            reg = dev->addr + (CR1 * 2) + 1;
            break;
        case MC68153_INT_2:
            reg = dev->addr + (CR2 * 2) + 1;
            break;
        case MC68153_INT_3:
            reg = dev->addr + (CR3 * 2) + 1;
            break;
        default:
            return; // Abort?
    }

    bits &= ~LEVEL_MASK; // Make sure the LEVEL_MASK bits are not set in the list of bits to clear.

    *(reg) &= ~bits; // Invert bits so that now all the set bits become masked off, the unset bits will stay the same.
}

/**
 * Sets the vector in VR(which) to <id>
 *
 */
static void MC68153SetVR(mc68153_dev_t * dev, mc68153_int_t which, uint8_t id)
{
    uint8_t * reg;

    switch (which)
    {
        case MC68153_INT_0:
            reg = dev->addr + (VR0 * 2) + 1;
            break;
        case MC68153_INT_1:
            reg = dev->addr + (VR1 * 2) + 1;
            break;
        case MC68153_INT_2:
            reg = dev->addr + (VR2 * 2) + 1;
            break;
        case MC68153_INT_3:
            reg = dev->addr + (VR3 * 2) + 1;
            break;
        default:
            return; // Abort?
    }

    *(reg) = id; 
}

/**
 * Setup one of the BIM interrupters.
 *
 */
void MC68153ConfigureInt(mc68153_dev_t * dev, mc68153_int_t which, bool setFlag, bool setFlagAutoClear, bool isExternal, uint8_t internalID, uint8_t lvl)
{
    uint8_t bits = 0;

    if (dev->isEnabled[which]) return; // Abort???

    if (setFlag) bits |= FLAG;
    if (setFlagAutoClear) bits |= FAC;
    if (isExternal) bits |= EXTERNAL;

    MC68153DisableInt(dev, which);
    MC68153SetCRBits(dev, which,  bits);
    MC68153SetCRLvl(dev, which, lvl);
    MC68153SetVR(dev, which, internalID);
}

/**
 * Enable interrupt
 *
 */
void MC68153EnableInt(mc68153_dev_t * dev, mc68153_int_t which, bool isAutoDisable)
{
    uint8_t bits = ENABLE;

    if (dev->isEnabled[which]) return; // Abort???

    if (isAutoDisable) bits |= IRAC;

    MC68153SetCRBits(dev, which, bits);

    dev->isEnabled[which] = true;
}

/**
 * Disable interrupt
 *
 */
void MC68153DisableInt(mc68153_dev_t * dev, mc68153_int_t which)
{
    if (!dev->isEnabled[which]) return; // Abort???

    MC68153ClrCRBits(dev, which, 0xFF); // Clear everything.

    dev->isEnabled[which] = false;
}

/* vim: set ai expandtab ts=4 sts=4 sw=4: */
