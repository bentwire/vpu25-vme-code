#ifndef __IO_H__
#define __IO_H__
#include <stdint.h>
#include <stddef.h>

/**
 * This is at the address where vme16 IO space starts
 *
 * This is set in the linker script.
 */
extern uint8_t __vme16_start[];

/**
 * This is at the address where vme32 space starts
 *
 * This is set in the linker script.
 */
extern uint8_t __vme32_start[];

static inline void copy_to_io8(uint8_t val, uint8_t *addr)
{
    asm volatile("move.b %0,(%1)\n\t": /* outputs */
                                      : /* inputs  */ "r"(val), "a"(addr)
                                      : /* clobbers */);
}

static inline uint8_t copy_from_io8(uint8_t *addr)
{
    uint8_t val;

    asm volatile("move.b (%1),%0\n\t": /* outputs */ "=r"(val)
                                      : /* inputs  */ "a"(addr)
                                      : /* clobbers */);

    return val;
}

static inline void copy_to_io16(uint16_t val, uint8_t *addr)
{
    asm volatile("movep.w %0,0(%1)\n\t": /* outputs */
                                      : /* inputs  */ "r"(val), "a"(addr)
                                      : /* clobbers */);
}

static inline uint16_t copy_from_io16(uint8_t *addr)
{
    uint16_t val;

    asm volatile("movep.w 0(%1),%0\n\t": /* outputs */ "=r"(val)
                                      : /* inputs  */ "a"(addr)
                                      : /* clobbers */);

    return val;
}

static inline void copy_to_io32(uint32_t val, uint8_t *addr)
{
    asm volatile("movep.l %0,0(%1)\n\t": /* outputs */
                                      : /* inputs  */ "r"(val), "a"(addr)
                                      : /* clobbers */);
}

static inline uint32_t copy_from_io32(uint8_t *addr)
{
    uint32_t val;

    asm volatile("movep.l 0(%1),%0\n\t": /* outputs */ "=r"(val)
                                      : /* inputs  */ "a"(addr)
                                      : /* clobbers */);

    return val;
}

#endif // __IO_H__
