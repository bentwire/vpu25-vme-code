#ifndef __VBIC_H__
#define __VBIC_H__

#include <stdint.h>
#include <stddef.h>
#include "IO.h"
#include "IRQ.h"

typedef struct _vbic_dev_t vbic_dev_t;

/**
 * Initialize VBIC and return pointer to vbic_dev_t.
 */
vbic_dev_t * VBICInit(uint8_t * vbic_base);

/**
 * Set the vector for the NMI switch.
 */
void VBICSetNMIVect(vbic_dev_t *dev, uint8_t vect);

/**
 * Set the vector for local interrupter 2
 */
void VBICSetLIR2Vect(vbic_dev_t *dev, uint8_t vect);

/**
 * Set the vector for local interrupter 3
 */
void VBICSetLIR3Vect(vbic_dev_t *dev, uint8_t vect);

/**
 * Set the vector for local interrupter 4
 */
void VBICSetLIR4Vect(vbic_dev_t *dev, uint8_t vect);

/**
 * Set the vector for local interrupter 5
 */
void VBICSetLIR5Vect(vbic_dev_t *dev, uint8_t vect);

/**
 * Set the vector for local interrupter 6
 */
void VBICSetLIR6Vect(vbic_dev_t *dev, uint8_t vect);

/**
 * Set the vector for local interrupter 7
 */
void VBICSetLIR7Vect(vbic_dev_t *dev, uint8_t vect);

/**
 * Set the vector for local interrupter 8
 */
void VBICSetLIR8Vect(vbic_dev_t *dev, uint8_t vect);

/**
 * Set the vector for local interrupter 9
 */
void VBICSetLIR9Vect(vbic_dev_t *dev, uint8_t vect);

/**
 * Set the vector for local interrupter 10
 */
void VBICSetLIR10Vect(vbic_dev_t *dev, uint8_t vect);

/**
 * Set the vector for VME Interrupt Generator
 */
void VBICSetVIGVect(vbic_dev_t *dev, uint8_t vect);

/**
 * Set the vector for Millisecond Marker.
 */
void VBICSetMSMVect(vbic_dev_t *dev, uint8_t vect);

/**
 * Set the vector for VME Interrupts.
 *
 * Lower 3 bits set by requesting VME IRQ (1-7).
 */
void VBICSetVMEVect(vbic_dev_t *dev, uint8_t vect);

void VBICInitMSM(vbic_dev_t *dev, irq_handler_t handler, uint32_t vec, uint8_t lvl); // This sets the MSM to 1ms interrupts, we could use 10 I suppose but 1 and 10 are the only choices...

/* vim: set ai expandtab ts=4 sts=4 sw=4: */
#endif //__VBIC_H__
