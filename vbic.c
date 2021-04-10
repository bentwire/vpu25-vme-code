#include "vbic.h"

struct _vbic_dev_t
{
    uint8_t * base_addr;
};

static vbic_dev_t VBIC; // There is only ever 1 VBIC.

typedef enum
{
    VECTNMI = 0x00,
    VECTLIR1,
    VECTLIR2,
    VECTLIR3,
    VECTLIR4,
    VECTLIR5,
    VECTLIR6,
    VECTLIR7,
    VECTLIR8,
    VECTLIR9,
    VECTLIR10,
    VECTVIG,
    VECTMSM,
    VECTVME,
    IRMASKA,
    IRMASKB,
    IRMASKC,
    IRSCANA,
    IRSCANB,
    IRSCANC,
    VBICCTL,

    ICLIR1 = 0x15,
    ICLIR2,
    ICLIR3,
    ICLIR4,
    ICLIR5,
    ICLIR6,
    ICLIR7,
    ICLIR8,
    ICLIR9,
    ICLIR10,
    ICVIR1,
    ICVIR2,
    ICVIR3,
    ICVIR4,
    ICVIR5,
    ICVIR6,
    ICVIR7,
    ICVIG,
    ICMSM,
    MSMCTL,

    VIRSID1 = 0x29,
    VIRSID2,
    VIRSID3,
    VIRSID4,
    VIRSID5,
    VIRSID6,
    VIRSID7,
    RSTCTL,

    VOUTCTL = 0x40,
    VOUTSID1,
    VOUTSID2,
    VOUTSID3,
    VOUTSID4,
    VOUTSID5,
    VOUTSID6,
    VOUTSID7,
    VIGCTL,
    VIASR,
    SRCLR,

    SLOTSTAT = 0x37,
    SYSCTL   = 0x4B,
} vbic_reg_off_t;

vbic_dev_t *VBICInit(uintptr_t vbic_base)
{
    VBIC.base_addr = (uint8_t *)vbic_base;
    return &VBIC;
}

/**
 * Set the vector for the NMI switch.
 */
void VBICSetNMIVect(vbic_dev_t *dev, uint8_t vect)
{
    uint8_t * vectreg  = dev->base_addr + (VECTNMI * 2) + 1;

    *vectreg = vect;
}

/**
 * Set the vector for local interrupter x
 *
 * @param lir which VECTLIR register to use.
 *
 * Argument checking performed upstream as this is an internal function.
 */
void VBICSetLIRxVect(vbic_dev_t *dev, vbic_reg_off_t lir, uint8_t vect)
{
    uint8_t * vectreg  = dev->base_addr + (lir * 2) + 1;

    *vectreg = vect;
}

/**
 * Set the vector for VME Interrupt Generator
 */
void VBICSetVIGVect(vbic_dev_t *dev, uint8_t vect)
{
    uint8_t * vectreg  = dev->base_addr + (VECTVIG * 2) + 1;

    *vectreg = vect;
}

/**
 * Set the vector for Millisecond Marker.
 */
void VBICSetMSMVect(vbic_dev_t *dev, uint8_t vect)
{
    uint8_t * vectreg  = dev->base_addr + (VECTMSM * 2) + 1;

    *vectreg = vect;
}

/**
 * Set the vector for VME Interrupts.
 *
 * Lower 3 bits set by requesting VME IRQ (1-7).
 */
void VBICSetVMEVect(vbic_dev_t *dev, uint8_t vect)
{
    uint8_t * vectreg  = dev->base_addr + (VECTVME * 2) + 1;

    *vectreg = vect;
}

/**
 * IRMASKA Bits
 *
 */
typedef enum
{
    MLIR1 = 0x02,
    MLIR2 = 0x04,
    MLIR3 = 0x08,
    MLIR4 = 0x10,
    MLIR5 = 0x20,
    MLIR6 = 0x40,
    MLIR7 = 0x80
} irmaska_bits_t;

/**
 * Set IRMASKA Bits
 *
 */
void VBICSetIRMASKABits(vbic_dev_t *dev, irmaska_bits_t bits)
{
    uint8_t * reg = dev->base_addr + (IRMASKA * 2) + 1;

    *reg |= (uint8_t)bits;
}

/**
 * Clear IRMASKA Bits
 *
 * @param mask Set bits to be cleared.
 */
void VBICClrIRMASKABits(vbic_dev_t *dev, irmaska_bits_t mask)
{
    uint8_t * reg = dev->base_addr + (IRMASKA * 2) + 1;

    *reg &= ((uint8_t)mask) ^ ((uint8_t)0xFF);
}

/**
 * IRMASKB Bits
 *
 */
typedef enum
{
    MLIR8  = 0x01,
    MLIR9  = 0x02,
    MLIR10 = 0x04,
    MMSM   = 0x40,
    MVIG   = 0x80
} irmaskb_bits_t;

/**
 * Set IRMASKB Bits
 *
 */
void VBICSetIRMASKBBits(vbic_dev_t *dev,  irmaskb_bits_t bits)
{
    uint8_t * reg = dev->base_addr + (IRMASKB * 2) + 1;

    *reg |= (uint8_t)bits;
}

/**
 * Clear IRMASKB Bits
 *
 * @param mask Set bits to be cleared.
 */
void VBICClrIRMASKBBits(vbic_dev_t *dev, uint8_t mask)
{
    uint8_t * reg = dev->base_addr + (IRMASKB * 2) + 1;

    *reg &= ((uint8_t)mask) ^ ((uint8_t)0xFF);
}

/**
 * IRMASKC Bits
 *
 */
typedef enum
{
    MVIR1 = 0x02,
    MVIR2 = 0x04,
    MVIR3 = 0x08,
    MVIR4 = 0x10,
    MVIR5 = 0x20,
    MVIR6 = 0x40,
    MVIR7 = 0x80
} irmaskc_bits_t;

/**
 * Set IRMASKC Bits
 *
 */
void VBICSetIRMASKCBits(vbic_dev_t *dev, irmaskc_bits_t bits)
{
    uint8_t * reg = dev->base_addr + (IRMASKC * 2) + 1;

    *reg |= (uint8_t)bits;
}

/**
 * Clear IRMASKC Bits
 *
 * @param mask Set bits to be cleared.
 */
void VBICClrIRMASKCBits(vbic_dev_t *dev, irmaskc_bits_t mask)
{
    uint8_t * reg = dev->base_addr + (IRMASKC * 2) + 1;

    *reg &= ((uint8_t)mask) ^ ((uint8_t)0xFF);
}

/**
 * ICLIRx Bits
 *
 */
typedef enum
{
    ACT_HI = 0x10,
    EDGE   = 0x20,
    EXTV   = 0x40,
    AUTOV  = 0x80
} iclirx_bits_t;

/**
 * Set ICLIRx bits
 */
void VBICSetICLIRxBits(vbic_dev_t *dev, vbic_reg_off_t which, iclirx_bits_t bits)
{
    switch (which) {
        case ICLIR1:
        case ICLIR2:
        case ICLIR3:
        case ICLIR4:
        case ICLIR5:
        case ICLIR6:
        case ICLIR7:
        case ICLIR8:
        case ICLIR9:
        case ICLIR10:
            {
                uint8_t * reg = dev->base_addr + (which * 2) + 1;

                bits &= 0xF0; // Make sure the level does not get clobbered
                *reg |= (uint8_t)bits;
            };
            break;
        default:
            return;
    }
}

/**
 * Clear ICLIRx bits
 */
void VBICClrICLIRxBits(vbic_dev_t *dev, vbic_reg_off_t which, iclirx_bits_t bits)
{
    switch (which) {
        case ICLIR1:
        case ICLIR2:
        case ICLIR3:
        case ICLIR4:
        case ICLIR5:
        case ICLIR6:
        case ICLIR7:
        case ICLIR8:
        case ICLIR9:
        case ICLIR10:
            {
                uint8_t * reg = dev->base_addr + (which * 2) + 1;

                bits &= 0xF0; // Make sure the level does not get clobbered
                *reg &= ((uint8_t)bits) ^ ((uint8_t)0xFF);
            };
            break;
        default:
            return;
    }
}

/**
 * Set ICLIRx irq level
 */
void VBICSetICLIRxLevel(vbic_dev_t *dev, vbic_reg_off_t which, uint8_t lvl)
{
    switch (which) {
        case ICLIR1:
        case ICLIR2:
        case ICLIR3:
        case ICLIR4:
        case ICLIR5:
        case ICLIR6:
        case ICLIR7:
        case ICLIR8:
        case ICLIR9:
        case ICLIR10:
            {
                uint8_t * reg = dev->base_addr + (which * 2) + 1;

                *reg &= (uint8_t)0xF8; // Clear the ILEV bits
                *reg |= lvl; // Set the level
            };
            break;
        default:
            return;
    }
}

/**
 * Set ICVIRx irq level
 *
 * Argument checking performed upstream as this is an internal function.
 */
void VBICSetICVIRxLevel(vbic_dev_t *dev, vbic_reg_off_t which, uint8_t lvl)
{
    uint8_t * reg = dev->base_addr + (which * 2) + 1;

    *reg &= (uint8_t)0xF8; // Clear the ILEV bits
    *reg |= lvl; // Set the level
}

/**
 * Set ICVIG irq level
 *
 * Argument checking performed upstream as this is an internal function.
 */
void VBICSetICVIGLevel(vbic_dev_t *dev, uint8_t lvl)
{
    uint8_t * reg = dev->base_addr + (ICVIG * 2) + 1;

    *reg &= (uint8_t)0xF8; // Clear the ILEV bits
    *reg |= lvl; // Set the level
}

/**
 * Set ICMSM irq level
 *
 * Argument checking performed upstream as this is an internal function.
 */
void VBICSetICMSMLevel(vbic_dev_t *dev, uint8_t lvl)
{
    uint8_t * reg = dev->base_addr + (ICMSM * 2) + 1;

    *reg &= (uint8_t)0xF8; // Clear the ILEV bits
    *reg |= lvl; // Set the level
}

/**
 * initMSM: Set up the millisecond marker timer
 *
 */
void VBICConfigMSM(vbic_dev_t *dev, irq_handler_t handler, uint32_t vec, uint8_t lvl) // This sets the MSM to 1ms interrupts, we could use 10 I suppose but 1 and 10 are the only choices...
{
    uint8_t * msmctl  = dev->base_addr + (MSMCTL * 2) + 1;

    *msmctl = 0x03; // Set freq to 1ms

    if (lvl > 7) lvl = 7;

    VBICSetICMSMLevel(dev, lvl); // Set interrupt level, between 1-7, 0 is disable.
    VBICSetMSMVect(dev, (uint8_t)(vec/4));

    *(uint32_t*)(vec) = (uint32_t)(handler);

    VBICSetIRMASKBBits(dev, MMSM);
}
/* vim: set ai expandtab ts=4 sts=4 sw=4: */
