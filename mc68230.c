#include "mc68230.h"

typedef enum 
{
    PGCR = 0,
    PSRR,
    PADDR,
    PBDDR,
    PCDDR,
    PIVR,
    PACR,
    PBCR,
    PADR,
    PBDR,
    PAAR,
    PBAR,
    PCDR,
    PSR,
    TCR = 0x10,
    TIVR,
    CPR = 0x12,
    CPRH = 0x13,
    CPRM,
    CPRL,
    CNTR = 0x16,
    CNTRH = 0x17,
    CNTRM,
    CNTRL,
    TSR,
} mc68230_reg_off_t;

/**
 * brief: Initialize PI/T pointed to by dev.
 *
 * Only touches regs affected by reset.
 */
void MC68230Init(mc68230_dev_t *dev)
{
    MC68230SetPGCR(dev, 0);
    MC68230SetPSRR(dev, 0);
    MC68230SetPADDR(dev, 0);
    MC68230SetPBDDR(dev, 0);
    MC68230SetPCDDR(dev, 0);
    MC68230SetPIVR(dev, 0x0F);
    MC68230SetPACR(dev, 0);
    MC68230SetPBCR(dev, 0);
    MC68230SetTCR(dev, 0);
    MC68230SetTIVR(dev, 0x0F); 
    MC68230SetTSR(dev, 0x01); // This is a reset operation. Writing this insures a value of 0x00 in the register.
}

/******** Port Section **********************************/

#define PGCR_MODE(a)    ((a << 6) & 0xC0)
#define PGCR_H34_ENABLE (1 << 5)
#define PGCR_H12_ENABLE (1 << 4)
#define PGCR_H4_SENSE   (1 << 3)
#define PGCR_H3_SENSE   (1 << 2)
#define PGCR_H2_SENSE   (1 << 1)
#define PGCR_H1_SENSE   (1 << 0)

/**
 * brief: Access PGCR
 *
 */
void MC68230SetPGCR(mc68230_dev_t *dev, uint8_t  val)
{
    // We multiply the register offset by 2 because they are at every other byte in the address map.
    // We add 1 to the overall offset so that the user does not have to pass in an odd base address.
    copy_to_io8(val, dev->addr + (PGCR * 2) + 1);
}

uint8_t MC68230GetPGCR(mc68230_dev_t *dev)
{
    // We multiply the register offset by 2 because they are at every other byte in the address map.
    // We add 1 to the overall offset so that the user does not have to pass in an odd base address.
    uint8_t val = copy_from_io8(dev->addr + (PGCR * 2) + 1);

    return val;
}

/**
 * brief: Access PSRR
 *
 */
void MC68230SetPSRR(mc68230_dev_t *dev, uint8_t  val)
{
    copy_to_io8(val, dev->addr + (PSRR * 2) + 1);
}

uint8_t MC68230GetPSRR(mc68230_dev_t *dev)
{
    uint8_t val = copy_from_io8(dev->addr + (PSRR * 2) + 1);

    return val;
}

/**
 * brief: Access PADDR
 *
 */
void MC68230SetPADDR(mc68230_dev_t *dev, uint8_t  val)
{
    copy_to_io8(val, dev->addr + (PADDR * 2) + 1);
}

uint8_t MC68230GetPADDR(mc68230_dev_t *dev)
{
    uint8_t val = copy_from_io8(dev->addr + (PADDR * 2) + 1);

    return val;
}

/**
 * brief: Access PBDDR
 *
 */
void MC68230SetPBDDR(mc68230_dev_t *dev, uint8_t  val)
{
    copy_to_io8(val, dev->addr + (PBDDR * 2) + 1);
}

uint8_t MC68230GetPBDDR(mc68230_dev_t *dev)
{
    uint8_t val = copy_from_io8(dev->addr + (PBDDR * 2) + 1);

    return val;
}

/**
 * brief: Access PCDDR
 *
 */
void MC68230SetPCDDR(mc68230_dev_t *dev, uint8_t  val)
{
    copy_to_io8(val, dev->addr + (PCDDR * 2) + 1);
}

uint8_t MC68230GetPCDDR(mc68230_dev_t *dev)
{
    uint8_t val = copy_from_io8(dev->addr + (PCDDR * 2) + 1);

    return val;
}

/**
 * brief: Access PIVR
 *
 */
void MC68230SetPIVR(mc68230_dev_t *dev, uint8_t  val)
{
    copy_to_io8(val, dev->addr + (PIVR * 2) + 1);
}

uint8_t MC68230GetPIVR(mc68230_dev_t *dev)
{
    uint8_t val = copy_from_io8(dev->addr + (PIVR * 2) + 1);

    return val;
}

/**
 * brief: Access PACR
 *
 */
void MC68230SetPACR(mc68230_dev_t *dev, uint8_t  val)
{
    copy_to_io8(val, dev->addr + (PACR * 2) + 1);
}

uint8_t MC68230GetPACR(mc68230_dev_t *dev)
{
    uint8_t val = copy_from_io8(dev->addr + (PACR * 2) + 1);

    return val;
}

/**
 * brief: Access PBCR
 *
 */
void MC68230SetPBCR(mc68230_dev_t *dev, uint8_t  val)
{
    copy_to_io8(val, dev->addr + (PBCR * 2) + 1);
}

uint8_t MC68230GetPBCR(mc68230_dev_t *dev)
{
    uint8_t val = copy_from_io8(dev->addr + (PBCR * 2) + 1);

    return val;
}

/**
 * brief: Access PADR
 *
 */
void MC68230SetPADR(mc68230_dev_t *dev, uint8_t  val)
{
    copy_to_io8(val, dev->addr + (PADR * 2) + 1);
}

uint8_t MC68230GetPADR(mc68230_dev_t *dev)
{
    uint8_t val = copy_from_io8(dev->addr + (PADR * 2) + 1);

    return val;
}

/**
 * brief: Access PBDR
 *
 */
void MC68230SetPBDR(mc68230_dev_t *dev, uint8_t  val)
{
    copy_to_io8(val, dev->addr + (PBDR * 2) + 1);
}

uint8_t MC68230GetPBDR(mc68230_dev_t *dev)
{
    uint8_t val = copy_from_io8(dev->addr + (PBDR * 2) + 1);

    return val;
}

/**
 * brief: Access PAAR
 *
 */
void MC68230SetPAAR(mc68230_dev_t *dev, uint8_t  val)
{
    copy_to_io8(val, dev->addr + (PAAR * 2) + 1);
}

uint8_t MC68230GetPAAR(mc68230_dev_t *dev)
{
    uint8_t val = copy_from_io8(dev->addr + (PAAR * 2) + 1);

    return val;
}

/**
 * brief: Access PBAR
 *
 */
void MC68230SetPBAR(mc68230_dev_t *dev, uint8_t  val)
{
    copy_to_io8(val, dev->addr + (PBAR * 2) + 1);
}

uint8_t MC68230GetPBAR(mc68230_dev_t *dev)
{
    uint8_t val = copy_from_io8(dev->addr + (PBAR * 2) + 1);

    return val;
}

/**
 * brief: Access PCDR
 *
 */
void MC68230SetPCDR(mc68230_dev_t *dev, uint8_t  val)
{
    copy_to_io8(val, dev->addr + (PCDR * 2) + 1);
}

uint8_t MC68230GetPCDR(mc68230_dev_t *dev)
{
    uint8_t val = copy_from_io8(dev->addr + (PCDR * 2) + 1);

    return val;
}

/**
 * brief: Access PSR
 *
 */
void MC68230SetPSR(mc68230_dev_t *dev, uint8_t  val)
{
    copy_to_io8(val, dev->addr + (PSR * 2) + 1);
}

uint8_t MC68230GetPSR(mc68230_dev_t *dev)
{
    uint8_t val = copy_from_io8(dev->addr + (PSR * 2) + 1);

    return val;
}

/******** Timer Section *********************************/

/**
 * brief: Access TCR
 *
 */
void MC68230SetTCR(mc68230_dev_t *dev, uint8_t  val)
{
    copy_to_io8(val, dev->addr + (TCR * 2) + 1);
}

uint8_t MC68230GetTCR(mc68230_dev_t *dev)
{
    uint8_t val = copy_from_io8(dev->addr + (TCR * 2) + 1);

    return val;
}

/**
 * brief: Access TIVR
 *
 */
void MC68230SetTIVR(mc68230_dev_t *dev, uint8_t  val)
{
    copy_to_io8(val, dev->addr + (TIVR * 2) + 1);
}

uint8_t MC68230GetTIVR(mc68230_dev_t *dev)
{
    uint8_t val = copy_from_io8(dev->addr + (TIVR * 2) + 1);

    return val;
}

/**
 * brief: Access CPR
 *
 * Counter Preload Register Access.
 */
void MC68230SetCPR(mc68230_dev_t *dev, uint32_t  val)
{
    copy_to_io32(val, dev->addr + (CPR * 2) + 1);
}

uint32_t MC68230GetCPR(mc68230_dev_t *dev)
{
    uint32_t val = copy_from_io32(dev->addr + (CPR * 2) + 1);

    return val;
}

/**
 * brief: Access CPRH
 *
 * Counter Preload Register Access.
 */
void MC68230SetCPRH(mc68230_dev_t *dev, uint8_t  val)
{
    copy_to_io8(val, dev->addr + (CPRH * 2) + 1);
}

uint8_t MC68230GetCPRH(mc68230_dev_t *dev)
{
    uint8_t val = copy_from_io8(dev->addr + (CPRH * 2) + 1);

    return val;
}

/**
 * brief: Access CPRM
 *
 * Counter Preload Register Access.
 */
void MC68230SetCPRM(mc68230_dev_t *dev, uint8_t  val)
{
    copy_to_io8(val, dev->addr + (CPRM * 2) + 1);
}

uint8_t MC68230GetCPRM(mc68230_dev_t *dev)
{
    uint8_t val = copy_from_io8(dev->addr + (CPRM * 2) + 1);

    return val;
}

/**
 * brief: Access CPRL
 *
 * Counter Preload Register Access.
 */
void MC68230SetCPRL(mc68230_dev_t *dev, uint8_t  val)
{
    copy_to_io8(val, dev->addr + (CPRL * 2) + 1);
}

uint8_t MC68230GetCPRL(mc68230_dev_t *dev)
{
    uint8_t val = copy_from_io8(dev->addr + (CPRL * 2) + 1);

    return val;
}

/**
 * brief: Access CNTR
 *
 * Counter register access.
 */
void MC68230SetCNTR(mc68230_dev_t *dev, uint32_t  val)
{
    copy_to_io32(val, dev->addr + (CNTR * 2) + 1);
}

uint32_t MC68230GetCNTR(mc68230_dev_t *dev)
{
    uint32_t val = copy_from_io32(dev->addr + (CNTR * 2) + 1);

    return val;
}

/**
 * brief: Access CNTRH
 *
 * Counter register access.
 */
void MC68230SetCNTRH(mc68230_dev_t *dev, uint8_t  val)
{
    copy_to_io8(val, dev->addr + (CNTRH * 2) + 1);
}

uint8_t MC68230GetCNTRH(mc68230_dev_t *dev)
{
    uint8_t val = copy_from_io8(dev->addr + (CNTRH * 2) + 1);

    return val;
}

/**
 * brief: Access CNTRM
 *
 * Counter register access.
 */
void MC68230SetCNTRM(mc68230_dev_t *dev, uint8_t  val)
{
    copy_to_io8(val, dev->addr + (CNTRM * 2) + 1);
}

uint8_t MC68230GetCNTRM(mc68230_dev_t *dev)
{
    uint8_t val = copy_from_io8(dev->addr + (CNTRM * 2) + 1);

    return val;
}

/**
 * brief: Access CNTRL
 *
 * Counter register access.
 */
void MC68230SetCNTRL(mc68230_dev_t *dev, uint8_t  val)
{
    copy_to_io8(val, dev->addr + (CNTRL * 2) + 1);
}

uint8_t MC68230GetCNTRL(mc68230_dev_t *dev)
{
    uint8_t val = copy_from_io8(dev->addr + (CNTRL * 2) + 1);

    return val;
}

/**
 * brief: Access TSR
 *
 */
void MC68230SetTSR(mc68230_dev_t *dev, uint8_t  val)
{
    copy_to_io8(val, dev->addr + (TSR * 2) + 1);
}

uint8_t MC68230GetTSR(mc68230_dev_t *dev)
{
    uint8_t val = copy_from_io8(dev->addr + (TSR * 2) + 1);

    return val;
}
