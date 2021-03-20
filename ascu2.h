#ifndef __ASCU2_H__
#define __ASCU2_H__

#include <stdint.h>
#include <stddef.h>
#include "IO.h"

//uint8_t * const ascu2_addr     = (uint8_t *)(0xFFFFF000); // Short IO Space Address 0xF000

/**
 * Initialize the basic functions on the ASCU2.
 *
 * This initializes both PITs and sets up the default VME bus error timeout as well as the bus clear timer.
 */
void ASCU2Init(void);

/**
 * Set bus clear timer.
 */
void ASCU2SetBClrTimeout(uint32_t timeout);

/**
 * Set the interrupt timer.
 */
void ASCU2SetTimer(uint32_t timeout);
#endif //__ASCU2_H__
