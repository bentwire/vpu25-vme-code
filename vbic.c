#include <stdio.h>
#include "vbic.h"

struct _vbic_dev_t
{
    uint8_t * base_addr;
};

#ifndef VBIC_VME_BASE_VEC
#define VBIC_VME_BASE_VEC 0x3E0
#endif

static vbic_dev_t VBIC; // There is only ever 1 VBIC.
static vme_irq_handler_t vme_handlers[256] = { NULL }; // The VME interrupt ID is 8 bits on this system.  This allows for 256 interrupt "IDs"

static void VBICInitVMEInts(vbic_dev_t *dev, irq_handler_t handler, uintptr_t vec);

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

__attribute__ ((interrupt))
static void vbic_vme_handler(uint16_t vec)
{
    uint8_t id;
    uint8_t irq;
    uint8_t virscan;
    uint8_t virmask;
    uint8_t virirqs;

    uint16_t save = disable_irqs();

    virscan = *(VBIC.base_addr + (IRSCANC * 2) + 1);
    virmask = *(VBIC.base_addr + (IRMASKC * 2) + 1);

    restore_irqs(save);

    virirqs = virscan & virmask; // Make sure we only use the enabled IRQ's.

    for (int shift = 0; shift < 7; shift++)
    {
        // We are going to arbitrarily say that VME IRQ 7 is of the highest priority.
        if (((virirqs << shift) & 0x80) == 0x80) // IRQ Fired, read ID from appropriate register
        {
            // Determine which VME IRQ and ID Fired
            irq = 7 - shift;
            id  = *(VBIC.base_addr + ((VIRSID1 + (irq - 1)) * 2) + 1);

            // Use the ID to get the handler address
            vme_irq_handler_t handler = vme_handlers[id];
            if (handler != NULL)
            {
                // Call the user handler with the vec and id
                handler(vec, id);
            }
            else
            {
                printf("SPURRIOUS VME INT: VEC: 0x%4.4x, IRQ: %d, ID: 0x%2.2x\n", vec, irq, id); 
            }
        }
    }
}

vbic_dev_t *VBICInit(uintptr_t vbic_base)
{
    VBIC.base_addr = (uint8_t *)vbic_base;

    VBICInitVMEInts(&VBIC, vbic_vme_handler, VBIC_VME_BASE_VEC);
    return &VBIC;
}

/**
 * Set the vector for the NMI switch.
 */
static void VBICSetNMIVect(vbic_dev_t *dev, uint8_t vect)
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
static void VBICSetLIRxVect(vbic_dev_t *dev, vbic_reg_off_t lir, uint8_t vect)
{
    uint8_t * vectreg  = dev->base_addr + (lir * 2) + 1;

    *vectreg = vect;
}

/**
 * Set the vector for VME Interrupt Generator
 */
static void VBICSetVIGVect(vbic_dev_t *dev, uint8_t vect)
{
    uint8_t * vectreg  = dev->base_addr + (VECTVIG * 2) + 1;

    *vectreg = vect;
}

/**
 * Set the vector for Millisecond Marker.
 */
static void VBICSetMSMVect(vbic_dev_t *dev, uint8_t vect)
{
    uint8_t * vectreg  = dev->base_addr + (VECTMSM * 2) + 1;

    *vectreg = vect;
}

/**
 * Set the vector for VME Interrupts.
 *
 * Lower 3 bits set by requesting VME IRQ (1-7).
 */
static void VBICSetVMEVect(vbic_dev_t *dev, uint8_t vect)
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
static void VBICSetIRMASKABits(vbic_dev_t *dev, irmaska_bits_t bits)
{
    uint8_t * reg = dev->base_addr + (IRMASKA * 2) + 1;

    *reg |= (uint8_t)bits;
}

/**
 * Clear IRMASKA Bits
 *
 * @param mask Set bits to be cleared.
 */
static void VBICClrIRMASKABits(vbic_dev_t *dev, irmaska_bits_t mask)
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
static void VBICSetIRMASKBBits(vbic_dev_t *dev,  irmaskb_bits_t bits)
{
    uint8_t * reg = dev->base_addr + (IRMASKB * 2) + 1;

    *reg |= (uint8_t)bits;
}

/**
 * Clear IRMASKB Bits
 *
 * @param mask Set bits to be cleared.
 */
static void VBICClrIRMASKBBits(vbic_dev_t *dev, uint8_t mask)
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
static void VBICSetIRMASKCBits(vbic_dev_t *dev, irmaskc_bits_t bits)
{
    uint8_t * reg = dev->base_addr + (IRMASKC * 2) + 1;

    *reg |= (uint8_t)bits;
}

/**
 * Clear IRMASKC Bits
 *
 * @param mask Set bits to be cleared.
 */
static void VBICClrIRMASKCBits(vbic_dev_t *dev, irmaskc_bits_t mask)
{
    uint8_t * reg = dev->base_addr + (IRMASKC * 2) + 1;

    *reg &= ((uint8_t)mask) ^ ((uint8_t)0xFF);
}

/**
 * Set IRMASKx Bits
 *
 */
static void VBICSetIRMASKxBits(vbic_dev_t *dev, vbic_reg_off_t which, uint8_t bits)
{
    uint8_t * reg = dev->base_addr + (which * 2) + 1;

    *reg |= bits;
}

/**
 * Clear IRMASKx Bits
 *
 * @param mask Set bits to be cleared.
 */
static void VBICClrIRMASKxBits(vbic_dev_t *dev, vbic_reg_off_t which, uint8_t mask)
{
    uint8_t * reg = dev->base_addr + (which * 2) + 1;

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
static void VBICSetICLIRxBits(vbic_dev_t *dev, vbic_reg_off_t which, iclirx_bits_t bits)
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
static void VBICClrICLIRxBits(vbic_dev_t *dev, vbic_reg_off_t which, iclirx_bits_t bits)
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
static void VBICSetICLIRxLevel(vbic_dev_t *dev, vbic_reg_off_t which, uint8_t lvl)
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
static void VBICSetICVIRxLevel(vbic_dev_t *dev, vbic_reg_off_t which, uint8_t lvl)
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
static void VBICSetICVIGLevel(vbic_dev_t *dev, uint8_t lvl)
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
static void VBICSetICMSMLevel(vbic_dev_t *dev, uint8_t lvl)
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

// TODO: Add Enable/DisableMSM

/**
 * Configure the NMI switch.
 */
void VBICConfigNMISwitch(vbic_dev_t *dev, irq_handler_t handler, uintptr_t vec)
{
    VBICSetNMIVect(dev, (uint8_t)(vec/4));

    *(uintptr_t*)(vec) = (uint32_t)(handler);
}

/**
 * Configure one of the local interrupters
 */
void VBICConfigLocalInt(vbic_dev_t *dev, lir_t which, irq_handler_t handler, uintptr_t vec, uint8_t lvl, bool isAuto, bool isExt, bool isEdge, bool isActHi)
{
    vbic_reg_off_t lirReg = 0;
    vbic_reg_off_t vectReg = 0;
    iclirx_bits_t iclirBits = 0;

    if (lvl > 7)
        lvl = 7;

    if (isAuto) iclirBits |= AUTOV;
    if (isExt) iclirBits |= EXTV;
    if (isEdge) iclirBits |= EDGE;
    if (isActHi) iclirBits |= ACT_HI;

    switch (which)
    {
        case LOCAL_INTERRUPTER_1:
            lirReg = ICLIR1;
            vectReg = VECTLIR1;
            break;
        case LOCAL_INTERRUPTER_2:
            lirReg = ICLIR2;
            vectReg = VECTLIR2;
            break;
        case LOCAL_INTERRUPTER_3:
            lirReg = ICLIR3;
            vectReg = VECTLIR3;
            break;
        case LOCAL_INTERRUPTER_4:
            lirReg = ICLIR4;
            vectReg = VECTLIR4;
            break;
        case LOCAL_INTERRUPTER_5:
            lirReg = ICLIR5;
            vectReg = VECTLIR5;
            break;
        case LOCAL_INTERRUPTER_6:
            lirReg = ICLIR6;
            vectReg = VECTLIR6;
            break;
        case LOCAL_INTERRUPTER_7:
            lirReg = ICLIR7;
            vectReg = VECTLIR7;
            break;
        case LOCAL_INTERRUPTER_8:
            lirReg = ICLIR8;
            vectReg = VECTLIR8;
            break;
        case LOCAL_INTERRUPTER_9:
            lirReg = ICLIR9;
            vectReg = VECTLIR9;
            break;
        case LOCAL_INTERRUPTER_10:
            lirReg = ICLIR10;
            vectReg = VECTLIR10;
            break;
    }

    if ((lirReg != 0) && (vectReg != 0))
    {
        *(uintptr_t*)(vec) = (uint32_t)(handler);

        VBICClrICLIRxBits(dev, lirReg, 0xF0);
        VBICSetICLIRxBits(dev, lirReg, iclirBits);
        VBICSetICLIRxLevel(dev, lirReg, lvl);
        VBICSetLIRxVect(dev, vectReg, (uint8_t)(vec/4));
    }
}

// TODO: Add Enable/DisableLocalInt
/**
 * Configure one of the local interrupters
 */
void VBICEnableLocalInt(vbic_dev_t *dev, lir_t which)
{
    switch (which)
    {
        case LOCAL_INTERRUPTER_1:
            VBICSetIRMASKABits(dev, MLIR1);
            break;
        case LOCAL_INTERRUPTER_2:
            VBICSetIRMASKABits(dev, MLIR2);
            break;
        case LOCAL_INTERRUPTER_3:
            VBICSetIRMASKABits(dev, MLIR3);
            break;
        case LOCAL_INTERRUPTER_4:
            VBICSetIRMASKABits(dev, MLIR4);
            break;
        case LOCAL_INTERRUPTER_5:
            VBICSetIRMASKABits(dev, MLIR5);
            break;
        case LOCAL_INTERRUPTER_6:
            VBICSetIRMASKABits(dev, MLIR6);
            break;
        case LOCAL_INTERRUPTER_7:
            VBICSetIRMASKABits(dev, MLIR7);
            break;
        case LOCAL_INTERRUPTER_8:
            VBICSetIRMASKBBits(dev, MLIR8);
            break;
        case LOCAL_INTERRUPTER_9:
            VBICSetIRMASKBBits(dev, MLIR9);
            break;
        case LOCAL_INTERRUPTER_10:
            VBICSetIRMASKBBits(dev, MLIR10);
            break;
    }
}

/**
 * Configure one of the local interrupters
 */
void VBICDisbleLocalInt(vbic_dev_t *dev, lir_t which)
{
    switch (which)
    {
        case LOCAL_INTERRUPTER_1:
            VBICClrIRMASKABits(dev, MLIR1);
            break;
        case LOCAL_INTERRUPTER_2:
            VBICClrIRMASKABits(dev, MLIR2);
            break;
        case LOCAL_INTERRUPTER_3:
            VBICClrIRMASKABits(dev, MLIR3);
            break;
        case LOCAL_INTERRUPTER_4:
            VBICClrIRMASKABits(dev, MLIR4);
            break;
        case LOCAL_INTERRUPTER_5:
            VBICClrIRMASKABits(dev, MLIR5);
            break;
        case LOCAL_INTERRUPTER_6:
            VBICClrIRMASKABits(dev, MLIR6);
            break;
        case LOCAL_INTERRUPTER_7:
            VBICClrIRMASKABits(dev, MLIR7);
            break;
        case LOCAL_INTERRUPTER_8:
            VBICClrIRMASKBBits(dev, MLIR8);
            break;
        case LOCAL_INTERRUPTER_9:
            VBICClrIRMASKBBits(dev, MLIR9);
            break;
        case LOCAL_INTERRUPTER_10:
            VBICClrIRMASKBBits(dev, MLIR10);
            break;
    }
}

static void VBICInitVMEInts(vbic_dev_t *dev, irq_handler_t handler, uintptr_t vec)
{
    *(uintptr_t*)(vec+4*0) = (uint32_t)(handler);
    *(uintptr_t*)(vec+4*1) = (uint32_t)(handler);
    *(uintptr_t*)(vec+4*2) = (uint32_t)(handler);
    *(uintptr_t*)(vec+4*3) = (uint32_t)(handler);
    *(uintptr_t*)(vec+4*4) = (uint32_t)(handler);
    *(uintptr_t*)(vec+4*5) = (uint32_t)(handler);
    *(uintptr_t*)(vec+4*6) = (uint32_t)(handler);
    *(uintptr_t*)(vec+4*7) = (uint32_t)(handler);

    VBICSetVMEVect(dev, (uint8_t)(vec/4) & 0xF8);
}

/**
 * Configure one of the VME interrupters
 *
 */
void VBICConfigVMEInt(vbic_dev_t *dev, vir_t which, vme_irq_handler_t handler, uint8_t id, uint8_t lvl)
{
    vbic_reg_off_t virReg = 0;

    if (lvl > 7)
        lvl = 7;

    switch (which)
    {
        case VME_INTERRUPTER_1:
            virReg = ICVIR1;
            break;
        case VME_INTERRUPTER_2:
            virReg = ICVIR2;
            break;
        case VME_INTERRUPTER_3:
            virReg = ICVIR3;
            break;
        case VME_INTERRUPTER_4:
            virReg = ICVIR4;
            break;
        case VME_INTERRUPTER_5:
            virReg = ICVIR5;
            break;
        case VME_INTERRUPTER_6:
            virReg = ICVIR6;
            break;
        case VME_INTERRUPTER_7:
            virReg = ICVIR7;
            break;
    }

    if ((virReg != 0))
    {
        VBICDisableVMEInt(dev, which);
        vme_handlers[id] = handler;

        VBICSetICVIRxLevel(dev, virReg, lvl);
    }
}

/**
 * Configure one of the VME interrupters
 */
void VBICEnableVMEInt(vbic_dev_t *dev, vir_t which)
{
    switch (which)
    {
        case VME_INTERRUPTER_1:
            VBICSetIRMASKCBits(dev, MVIR1);
            break;
        case VME_INTERRUPTER_2:
            VBICSetIRMASKCBits(dev, MVIR2);
            break;
        case VME_INTERRUPTER_3:
            VBICSetIRMASKCBits(dev, MVIR3);
            break;
        case VME_INTERRUPTER_4:
            VBICSetIRMASKCBits(dev, MVIR4);
            break;
        case VME_INTERRUPTER_5:
            VBICSetIRMASKCBits(dev, MVIR5);
            break;
        case VME_INTERRUPTER_6:
            VBICSetIRMASKCBits(dev, MVIR6);
            break;
        case VME_INTERRUPTER_7:
            VBICSetIRMASKCBits(dev, MVIR7);
            break;
    }
}

/**
 * Configure one of the VME interrupters
 */
void VBICDisableVMEInt(vbic_dev_t *dev, vir_t which)
{
    switch (which)
    {
        case VME_INTERRUPTER_1:
            VBICClrIRMASKCBits(dev, MVIR1);
            break;
        case VME_INTERRUPTER_2:
            VBICClrIRMASKCBits(dev, MVIR2);
            break;
        case VME_INTERRUPTER_3:
            VBICClrIRMASKCBits(dev, MVIR3);
            break;
        case VME_INTERRUPTER_4:
            VBICClrIRMASKCBits(dev, MVIR4);
            break;
        case VME_INTERRUPTER_5:
            VBICClrIRMASKCBits(dev, MVIR5);
            break;
        case VME_INTERRUPTER_6:
            VBICClrIRMASKCBits(dev, MVIR6);
            break;
        case VME_INTERRUPTER_7:
            VBICClrIRMASKCBits(dev, MVIR7);
            break;
    }
}

/* vim: set ai expandtab ts=4 sts=4 sw=4: */
