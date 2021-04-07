#ifndef __VACS_H__
#define __VACS_H__
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/**
 * Set the VACS configuration for the 1M address range starting at address given in addr.
 *
 * @param addr The address to update the VACS for.
 * @param isA24 Is this 1M address range for A24 or A32 access.
 * @param isAT  Does this address range support only Aligned Transfers?
 * @param isD16 Is this 1M address range D16 accessible or D32?
 */
static inline void VACSv1SetForAddress(void *addr, bool isA24, bool isAT, bool isD16)
{
    uint8_t * vacs = (uint8_t *)(0xFFFB0000);
    uint32_t offset = (uint32_t)addr >> 20;

    uint8_t val = 0x00;

    if (isA24) val |= 0x04;
    if (isAT)  val |= 0x02;
    if (isD16) val |= 0x01;

    *(vacs + offset) =  val;
}

/**
 * Set the VACS configuration for the 16M address range starting at address given in addr.
 *
 * @param addr The address to update the VACS for.
 * @param isA24 Is this 16M address range for A24 or A32 access.
 * @param isAT  Does this address range support only Aligned Transfers?
 * @param isD16 Is this 16M address range D16 accessible or D32?
 */
static inline void VACSv2SetForAddress(void *addr, bool isA24, bool isAT, bool isD16)
{
    uint8_t * vacs   = (uint8_t *)(0x00FB0000);
    uint32_t  offset = (uint32_t)addr & 0xFF000000;
    uint8_t * csr    = (uint8_t *)(0xFFFA0000);
    uint8_t   val    = 0x00;

    *csr = 0x20; // Disable VME bus access.


    if (isA24) val |= 0x04;
    if (isAT)  val |= 0x02;
    if (isD16) val |= 0x01;

    *(vacs + offset) =  val;

    *csr = 0x10; // Enable VME bus access.
}

/* vim: set ai expandtab ts=4 sts=4 sw=4: */
#endif
