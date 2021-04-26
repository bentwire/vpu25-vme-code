#ifndef __IRQ_H__
#define __IRQ_H__

typedef void (*irq_handler_t)(uint16_t vect);
typedef void (*vme_irq_handler_t)(uint16_t vect, uint8_t id);

static inline void enable_irqs(void)
{
    asm volatile("and.w #0xf0ff,%sr"); // Enable interrupts
}

static inline uint16_t disable_irqs(void)
{
    uint16_t reg;

    // save sr.
    asm volatile("move.w %%sr,%0\n\t" : /* outputs */ "=r"(reg)
                                      : /* inputs  */
                                      : /* clobbers */);
    // Disable interrupts.
    asm volatile("or.w #0x0700,%sr"); // Disable interrupts

    return reg;
}

static inline void restore_irqs(uint16_t sr)
{
    // restore sr.
    asm volatile("move.w %0,%%sr\n\t" : /* outputs */
                                     : /* inputs  */  "r" (sr)
                                     : /* clobbers */);

}

#endif
/* vim: set ai expandtab ts=4 sts=4 sw=4: */
