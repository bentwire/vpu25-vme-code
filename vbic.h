#ifndef __VBIC_H__
#define __VBIC_H__

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "IO.h"
#include "IRQ.h"

typedef struct _vbic_dev_t vbic_dev_t;

/**
 * Initialize VBIC and return pointer to vbic_dev_t.
 */
vbic_dev_t * VBICInit(uintptr_t vbic_base);


void VBICConfigMSM(vbic_dev_t *dev, irq_handler_t handler, uintptr_t vec, uint8_t lvl); // This sets the MSM to 1ms interrupts, we could use 10 I suppose but 1 and 10 are the only choices...

/**
 * Type defining the available local interrupters
 */
typedef enum
{
    LOCAL_INTERRUPTER_1,
    LOCAL_INTERRUPTER_2,
    LOCAL_INTERRUPTER_3,
    LOCAL_INTERRUPTER_4,
    LOCAL_INTERRUPTER_5,
    LOCAL_INTERRUPTER_6,
    LOCAL_INTERRUPTER_7,
    LOCAL_INTERRUPTER_8,
    LOCAL_INTERRUPTER_9,
    LOCAL_INTERRUPTER_10
} lir_t;

/**
 * Type defining the available VME interrupters
 */
typedef enum
{
    VME_INTERRUPTER_1,
    VME_INTERRUPTER_2,
    VME_INTERRUPTER_3,
    VME_INTERRUPTER_4,
    VME_INTERRUPTER_5,
    VME_INTERRUPTER_6,
    VME_INTERRUPTER_7,
} vir_t;

/**
 * Configure the NMI switch.
 */
void VBICConfigNMISwitch(vbic_dev_t *dev, irq_handler_t handler, uintptr_t vec);

/**
 * Configure one of the local interrupters
 */
void VBICConfigLocalInt(vbic_dev_t *dev, lir_t which, irq_handler_t handler, uintptr_t vec, uint8_t lvl, bool isAuto, bool isExt, bool isEdge, bool isActHi);

/**
 * Configure one of the VME interrupters
 */
void VBICConfigVMEInt(vbic_dev_t *dev, vir_t which, irq_handler_t handler, uintptr_t vec, uint8_t lvl);

/* vim: set ai expandtab ts=4 sts=4 sw=4: */
#endif //__VBIC_H__
