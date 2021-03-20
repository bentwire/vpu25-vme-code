#ifndef __MC68230_H__
#define __MC68230_H__

#include <stdint.h>
#include <stddef.h>

#include "IO.h"

typedef struct
{
    uint8_t *addr;  // PI/T base address.
} mc68230_dev_t;

/**
 * brief: Initialize PI/T pointed to by dev.
 *
 * Sets all ports to INPUT and sets vectors to 0x0F.
 * Initializes all port data regs to 0x00.
 *
 */
void MC68230Init(mc68230_dev_t *dev);

/******** Port Section **********************************/

/**
 * brief: Access PGCR
 *
 */
void MC68230SetPGCR(mc68230_dev_t *dev, uint8_t  val);
uint8_t MC68230GetPGCR(mc68230_dev_t *dev);

/**
 * brief: Access PSRR
 *
 */
void MC68230SetPSRR(mc68230_dev_t *dev, uint8_t  val);
uint8_t MC68230GetPSRR(mc68230_dev_t *dev);

/**
 * brief: Access PADDR
 *
 */
void MC68230SetPADDR(mc68230_dev_t *dev, uint8_t  val);
uint8_t MC68230GetPADDR(mc68230_dev_t *dev);

/**
 * brief: Access PBDDR
 *
 */
void MC68230SetPBDDR(mc68230_dev_t *dev, uint8_t  val);
uint8_t MC68230GetPBDDR(mc68230_dev_t *dev);

/**
 * brief: Access PCDDR
 *
 */
void MC68230SetPCDDR(mc68230_dev_t *dev, uint8_t  val);
uint8_t MC68230GetPCDDR(mc68230_dev_t *dev);

/**
 * brief: Access PIVR
 *
 */
void MC68230SetPIVR(mc68230_dev_t *dev, uint8_t  val);
uint8_t MC68230GetPIVR(mc68230_dev_t *dev);

/**
 * brief: Access PACR
 *
 */
void MC68230SetPACR(mc68230_dev_t *dev, uint8_t  val);
uint8_t MC68230GetPACR(mc68230_dev_t *dev);

/**
 * brief: Access PBCR
 *
 */
void MC68230SetPBCR(mc68230_dev_t *dev, uint8_t  val);
uint8_t MC68230GetPBCR(mc68230_dev_t *dev);

/**
 * brief: Access PADR
 *
 */
void MC68230SetPADR(mc68230_dev_t *dev, uint8_t  val);
uint8_t MC68230GetPADR(mc68230_dev_t *dev);

/**
 * brief: Access PBDR
 *
 */
void MC68230SetPBDR(mc68230_dev_t *dev, uint8_t  val);
uint8_t MC68230GetPBDR(mc68230_dev_t *dev);

/**
 * brief: Access PAAR
 *
 */
void MC68230SetPAAR(mc68230_dev_t *dev, uint8_t  val);
uint8_t MC68230GetPAAR(mc68230_dev_t *dev);

/**
 * brief: Access PBAR
 *
 */
void MC68230SetPBAR(mc68230_dev_t *dev, uint8_t  val);
uint8_t MC68230GetPBAR(mc68230_dev_t *dev);

/**
 * brief: Access PCDR
 *
 */
void MC68230SetPCDR(mc68230_dev_t *dev, uint8_t  val);
uint8_t MC68230GetPCDR(mc68230_dev_t *dev);

/**
 * brief: Access PSR
 *
 */
void MC68230SetPSR(mc68230_dev_t *dev, uint8_t  val);
uint8_t MC68230GetPSR(mc68230_dev_t *dev);


/******** Timer Section *********************************/

/**
 * brief: Access TCR
 *
 */
void MC68230SetTCR(mc68230_dev_t *dev, uint8_t  val);
uint8_t MC68230GetTCR(mc68230_dev_t *dev);

/**
 * brief: Access TIVR
 *
 */
void MC68230SetTIVR(mc68230_dev_t *dev, uint8_t  val);
uint8_t MC68230GetTIVR(mc68230_dev_t *dev);

/**
 * brief: Access CPR
 *
 * Counter Preload Register Access.
 */
void MC68230SetCPR(mc68230_dev_t *dev, uint32_t  val);
uint32_t MC68230GetCPR(mc68230_dev_t *dev);

/**
 * brief: Access CPRH
 *
 * Counter Preload Register Access.
 */
void MC68230SetCPRH(mc68230_dev_t *dev, uint8_t  val);
uint8_t MC68230GetCPRH(mc68230_dev_t *dev);

/**
 * brief: Access CPRM
 *
 * Counter Preload Register Access.
 */
void MC68230SetCPRM(mc68230_dev_t *dev, uint8_t  val);
uint8_t MC68230GetCPRM(mc68230_dev_t *dev);

/**
 * brief: Access CPRL
 *
 * Counter Preload Register Access.
 */
void MC68230SetCPRL(mc68230_dev_t *dev, uint8_t  val);
uint8_t MC68230GetCPRL(mc68230_dev_t *dev);

/**
 * brief: Access CNTR
 *
 * Counter register access.
 */
void MC68230SetCNTR(mc68230_dev_t *dev, uint32_t  val);
uint32_t MC68230GetCNTR(mc68230_dev_t *dev);

/**
 * brief: Access CNTRH
 *
 * Counter register access.
 */
void MC68230SetCNTRH(mc68230_dev_t *dev, uint8_t  val);
uint8_t MC68230GetCNTRH(mc68230_dev_t *dev);

/**
 * brief: Access CNTRM
 *
 * Counter register access.
 */
void MC68230SetCNTRM(mc68230_dev_t *dev, uint8_t  val);
uint8_t MC68230GetCNTRM(mc68230_dev_t *dev);

/**
 * brief: Access CNTRL
 *
 * Counter register access.
 */
void MC68230SetCNTRL(mc68230_dev_t *dev, uint8_t  val);
uint8_t MC68230GetCNTRL(mc68230_dev_t *dev);

/**
 * brief: Access TSR
 *
 */
void MC68230SetTSR(mc68230_dev_t *dev, uint8_t  val);
uint8_t MC68230GetTSR(mc68230_dev_t *dev);

#endif // __MC68230_H__
