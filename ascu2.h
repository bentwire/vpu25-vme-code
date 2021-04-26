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
 * Configure nSYSFAIL interrupt.
 */
void ASCU2ConfigureSysFailInt(ascu2_dev_t * dev, uint8_t id, uint8_t lvl);

/**
 * Enable nSYSFAIL interrupt.
 */
void ASCU2EnableSysFailInt(ascu2_dev_t * dev);

/**
 * Disable nSYSFAIL interrupt.
 */
void ASCU2DisableSysFailInt(ascu2_dev_t * dev);

/**
 * Configure nACFAIL interrupt.
 */
void ASCU2ConfigureACFailInt(ascu2_dev_t * dev, uint8_t id, uint8_t lvl);

/**
 * Enable nACFAIL interrupt.
 */
void ASCU2EnableACFailInt(ascu2_dev_t * dev);

/**
 * Disable nACFAIL interrupt.
 */
void ASCU2DisableACFailInt(ascu2_dev_t * dev);

/**
 * Configure TEST interrupt.
 */
void ASCU2ConfigureTESTInt(ascu2_dev_t * dev, uint8_t id, uint8_t lvl);

/**
 * Enable TEST interrupt.
 */
void ASCU2EnableTESTInt(ascu2_dev_t * dev);

/**
 * Disable TEST interrupt.
 */
void ASCU2DisableTESTInt(ascu2_dev_t * dev);

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
