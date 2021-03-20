#ifndef __VBIC_H__
#define __VBIC_H__

#include <stdint.h>
#include <stddef.h>
#include "IO.h"

struct _vbic_dev_t;

/**
 * Set the vector for the NMI switch.
 */
void VBICSetNMIVect(uint8_t vect);

/**
 * Set the vector for local interrupter 2
 */
void VBICSetLIR2Vect(uint8_t vect);

/**
 * Set the vector for local interrupter 3
 */
void VBICSetLIR3Vect(uint8_t vect);

/**
 * Set the vector for local interrupter 4
 */
void VBICSetLIR4Vect(uint8_t vect);

/**
 * Set the vector for local interrupter 5
 */
void VBICSetLIR5Vect(uint8_t vect);

/**
 * Set the vector for local interrupter 6
 */
void VBICSetLIR6Vect(uint8_t vect);

/**
 * Set the vector for local interrupter 7
 */
void VBICSetLIR7Vect(uint8_t vect);

/**
 * Set the vector for local interrupter 8
 */
void VBICSetLIR8Vect(uint8_t vect);

/**
 * Set the vector for local interrupter 9
 */
void VBICSetLIR9Vect(uint8_t vect);

/**
 * Set the vector for local interrupter 10
 */
void VBICSetLIR10Vect(uint8_t vect);

/**
 * Set the vector for VME Interrupt Generator
 */
void VBICSetVIGVect(uint8_t vect);

/**
 * Set the vector for Millisecond Marker.
 */
void VBICSetMSMVect(uint8_t vect);

/**
 * Set the vector for VME Interrupts.
 *
 * Lower 3 bits set by requesting VME IRQ (1-7).
 */
void VBICSetVMEVect(uint8_t vect);

typedef enum 
{
    MLIR1,
    MLIR2,
    MLIR3,
    MLIR4,
    MLIR5,
    MLIR6,
    MLIR7,
    MLIR8,
    MLIR9,
    MLIR10,
    MMSM,
    MVIG,
    MVIR7,
    MVIR6,
    MVIR5,
    MVIR4,
    MVIR3,
    MVIR2,
    MVIR1,
} vbic_irq_mask_t;


#endif //__VBIC_H__
