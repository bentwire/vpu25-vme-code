#ifndef __MC68153_BIM_H__
#define __MC68153_BIM_H__

#include <stdint.h>
#include <stdbool.h>

/**
 * Device struct.
 */
typedef struct _mc68153_dev_t mc68153_dev_t;

/**
 * Enum defining the ints in the BIM.
 *
 */
typedef enum
{
    MC68153_INT_0,
    MC68153_INT_1,
    MC68153_INT_2,
    MC68153_INT_3,
    MC68153_INT_MAX,
} mc68153_int_t;

/**
 * Get pointer to a BIM dev at address.
 *
 */
mc68153_dev_t * MC68153Init(uint8_t * const addr);

/**
 * Setup one of the BIM interrupters.
 *
 */
void MC68153ConfigureInt(mc68153_dev_t * dev, mc68153_int_t which, bool setFlag, bool setFlagAutoClear, bool isExternal, uint8_t internalID, uint8_t lvl);

/**
 * Enable interrupt
 *
 */
void MC68153EnableInt(mc68153_dev_t * dev, mc68153_int_t which, bool isAutoDisable);

/**
 * Disable interrupt
 *
 */
void MC68153DisableInt(mc68153_dev_t * dev, mc68153_int_t which);

#endif
/* vim: set ai expandtab ts=4 sts=4 sw=4: */
