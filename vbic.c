#include "vbic.h"

struct _vbic_dev_t
{
    uint8_t * base_addr;
    uint8_t * vectnmi;
    uint8_t * vectlir1;
    uint8_t * vectlir2;
    uint8_t * vectlir3;
    uint8_t * vectlir4;
    uint8_t * vectlir5;
    uint8_t * vectlir6;
    uint8_t * vectlir7;
    uint8_t * vectlir8;
    uint8_t * vectlir9;
    uint8_t * vectlir10;
    uint8_t * vectvig;
    uint8_t * vectmsm;
    uint8_t * vectvme;
    uint8_t * irmaska;
    uint8_t * irmaskb;
    uint8_t * irmaskc;
    uint8_t * irscana;
    uint8_t * irscanb;
    uint8_t * irscanc;
    uint8_t * vbicctl;
    uint8_t * msmctl;
    uint8_t * icmsm;
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

vbic_dev_t *VBICInit(uint8_t * vbic_base)
{
    VBIC.base_addr = vbic_base;
    VBIC.vectnmi   = VBIC.base_addr + (VECTNMI * 2) + 1;
    VBIC.vectlir1  = VBIC.base_addr + (VECTLIR1 * 2) + 1;
    VBIC.vectlir2  = VBIC.base_addr + (VECTLIR2 * 2) + 1;
    VBIC.vectlir3  = VBIC.base_addr + (VECTLIR3 * 2) + 1;
    VBIC.vectlir4  = VBIC.base_addr + (VECTLIR4 * 2) + 1;
    VBIC.vectlir5  = VBIC.base_addr + (VECTLIR5 * 2) + 1;
    VBIC.vectlir6  = VBIC.base_addr + (VECTLIR6 * 2) + 1;
    VBIC.vectlir7  = VBIC.base_addr + (VECTLIR7 * 2) + 1;
    VBIC.vectlir8  = VBIC.base_addr + (VECTLIR8 * 2) + 1;
    VBIC.vectlir9  = VBIC.base_addr + (VECTLIR9 * 2) + 1;
    VBIC.vectlir10 = VBIC.base_addr + (VECTLIR10 * 2) + 1;
    VBIC.vectvig   = VBIC.base_addr + (VECTVIG * 2) + 1;
    VBIC.vectmsm   = VBIC.base_addr + (VECTMSM * 2) + 1;
    VBIC.vectvme   = VBIC.base_addr + (VECTVME * 2) + 1;
    VBIC.msmctl    = VBIC.base_addr + (MSMCTL * 2) + 1;
    return &VBIC;
}

/**
 * initMSM: Set up the millisecond marker timer
 *
 */
void VBICInitMSM(vbic_dev_t *dev, irq_handler_t handler, irq_handler_t vec, uint8_t lvl) // This sets the MSM to 1ms interrupts, we could use 10 I suppose but 1 and 10 are the only choices...
{
    uint8_t * msmctl  = dev->base_addr + (MSMCTL * 2) + 1;
    uint8_t * icmsm   = dev->base_addr + (ICMSM * 2) + 1;
    uint8_t * vectmsm = dev->base_addr + (VECTMSM * 2) + 1;
    uint8_t * irmaskb = dev->base_addr + (IRMASKB * 2) + 1;

    *msmctl = 0x03; // Set freq to 1ms
    if (lvl > 7) lvl = 7;
    *icmsm = lvl; // Set interrupt level, between 1-7, 0 is disable.
    *vectmsm = (uint32_t)(vec)/4;  // This is very unsafe!  It relies on pointers being 32bits.
    *(uint32_t*)(vec) = (uint32_t)(handler);
    *irmaskb |= 0x40; // unmask MSM irq.
}
