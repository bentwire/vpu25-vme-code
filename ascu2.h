#ifndef __ASCU2_H__
#define __ASCU2_H__

#include <stdint.h>
#include <stddef.h>
#include "IO.h"

typedef struct _ascu2_dev_t ascu2_dev_t; // Empty declaration for encapsulation.  Only the driver should care what is inside.

/**
 * Initialize the basic functions on the ASCU2.
 *
 * This initializes both PITs and sets up the default VME bus error timeout as well as the bus clear timer.
 */
ascu2_dev_t * ASCU2Init(uint8_t * const base);

/**
 * Set bus clear timer.
 */
void ASCU2SetBClrTimeout(ascu2_dev_t * dev, uint32_t timeout);

/**
 * Set the interrupt timer.
 */
void ASCU2SetTimer(ascu2_dev_t * dev, uint32_t timeout);
#endif //__ASCU2_H__
/* vim: set ai expandtab ts=4 sts=4 sw=4: */
