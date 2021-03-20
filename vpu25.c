#include <stdio.h>
#include "vpu25.h"
#include "vbic.h"
#include "mc68230.h"
#include "ascu2.h"

static uint8_t * const           sysctl  = (uint8_t *)(0xfff90097);
static uint8_t * const           PIT     = (uint8_t *)(0xFFF88000);
static volatile uint32_t         ticker  = 0;
static volatile uint32_t         counter = 0;
static volatile uint32_t * const vmecntr = (uint32_t *)(0x10000000); // In the 256M ram card.

static mc68230_dev_t pit; // The PIT controls the LEDs, the dip switches, the interrupts from the 2 IP slots, and a status reg.

void VPU25SetBoardLEDs(uint8_t val)
{
    MC68230SetPBDR(&pit, val ^ 0xFF); // LEDs states are inverted.
}

__attribute__ ((interrupt)) void timer_isr(uint16_t vect)
{
    ticker++;
    vmecntr[0]++;
    vmecntr[1] = vect;
    if (counter > 0)
    {
        counter--;
    }
    VPU25SetBoardLEDs(vmecntr[0] >> 6);
}

static void sleepms(uint32_t ms)
{
    counter = ms;
    while (counter);
}

static void BoardInitPIT()
{
    pit.addr = PIT;

    MC68230Init(&pit);
    MC68230SetPGCR(&pit, 0x00);  // Port A and B in mode 0
    MC68230SetPSRR(&pit, 0x18);  // Port C, set to general IO, bit 6, 5 are PIT functions
                                 // Int IP-A (H1) has priority over IP-B (H2).
    MC68230SetPBDR(&pit, 0xFF);  // LED's are off when set to 1
    MC68230SetPCDR(&pit, 0x96);  // Bus error status bits and parity error status bits are
                                 // reset and disabled, SYSFAIL asserted.
    MC68230SetPADDR(&pit, 0x00); // Port A is set to all inputs for the 8 switches.
    MC68230SetPBDDR(&pit, 0xFF); // Port B is set to all outputs for the 8 LEDs.
    MC68230SetPCDDR(&pit, 0x97); // Port C bits 7, 4, 2, 1, 0 are outputs, 6, 5, 3 are inputs.
    MC68230SetPACR(&pit, 0x86);  // Port A in submode 1x, making H1 and H2 lines to IP-A and IP-B ints.
    MC68230SetPBCR(&pit, 0x80);  // Port B in submode 1x, H3 and H4 are disabled.
    MC68230SetTCR(&pit, 0xC0);   // Timer off. Port C bit 3 configured as timer int req (autovec)

    MC68230SetPCDR(&pit, MC68230GetPCDR(&pit) | 0x80);  // Insure parity test is disabled.
}

void BoardDeAssertSysFail(void)
{
    MC68230SetPCDR(&pit, MC68230GetPCDR(&pit) & ~0x02);  // Deassert SYSFAIL by setting bit 1 to 0.
}

void BoardAssertSysFail(void)
{
    MC68230SetPCDR(&pit, MC68230GetPCDR(&pit) | 0x02);  // Assert SYSFAIL by setting bit 1 to 1.
}

/**
 *   VME_BUS_TIMEOUT_16US    = 0 << 6,
 *   VME_BUS_TIMEOUT_32US    = 1 << 6,
 *   VME_BUS_TIMEOUT_64US    = 2 << 6,
 *   VME_BUS_TIMEOUT_128US   = 3 << 6,
 *   VME_BUS_TIMEOUT_MASK    = 0x3f,
 *   VME_BUS_ARB_RRS         = 1 << 4,
 *   VME_BUS_ARB_PRRS        = 2 << 4,
 *   VME_BUS_ARB_PRI         = 3 << 4,
 *   VME_BUS_ARB_MASK        = 0xcf,
 *   VME_BUS_BR_ROR          = 0 << 1,
 *   VME_BUS_BR_RWD          = 1 << 1,
 *   VME_BUS_BR_MASK         = 0xfd,
 **/

int main(void)
{
    printf("HI\r\n");

    BoardInitPIT();
    BoardSetVMEBusReqLvl(0);                       // Bus req level 0
    BoardSetVMEBusRelMethod(VME_BUS_BR_ROR);       // Release on request.
    // These only have an effect if the board is a system controller.
    //setVMEBusArbMethod(VME_BUS_ARB_PRRS);     // Priority round robin, br3 is highest prio, 2,1,0 are round robin.
    //setVMEBusArbMethod(VME_BUS_ARB_PRI);     // Priority, br3 is highest prio, followed by 2,1,0.
    //setVMEBusTimeout(VME_BUS_TIMEOUT_16US);  // 128us Timeout.

    BoardInitMSM(timer_isr, (irq_handler_t)(0x100), 2);

    BoardDeAssertSysFail();

    asm volatile("and.w #0xf0ff,%sr"); // Enable interrupts

    while(1)
    {
      printf("Ticks since start: %ld\r\n", ticker);
      sleepms(1000);
    }
    printf("DONE\r\n");
    return 0;
}

/**
 * Set VME Bus Request level (0-3)
 */
void BoardSetVMEBusReqLvl(uint8_t lvl)
{

    if (lvl > 3) lvl = 3;

    *sysctl &= 0xf3;
    *sysctl |= lvl << 2;
}

void BoardSetVMEBusRelMethod(sysctl_bits_t method)
{
    switch (method) {
        case VME_BUS_BR_ROR:
            *sysctl &= VME_BUS_BR_MASK;
            *sysctl |= VME_BUS_BR_ROR;
            break;

        case VME_BUS_BR_RWD:
            *sysctl &= VME_BUS_BR_MASK;
            *sysctl |= VME_BUS_BR_RWD;
            break;
        default:
            // Someone passed in something dumb,
            // lets default to Release When Done
            *sysctl &= VME_BUS_BR_MASK;
            *sysctl |= VME_BUS_BR_RWD;
            break;
    } 

}

/*
 *   VME_BUS_ARB_RRS         = 1 << 4,
 *   VME_BUS_ARB_PRRS        = 2 << 4,
 *   VME_BUS_ARB_PRI         = 3 << 4,
 */
void BoardSetVMEBusArbMethod(sysctl_bits_t method)
{
    switch (method) {
        case VME_BUS_ARB_RRS:
            *sysctl &= VME_BUS_ARB_MASK;
            *sysctl |= VME_BUS_ARB_RRS;
            break;

        case VME_BUS_ARB_PRRS:
            *sysctl &= VME_BUS_ARB_MASK;
            *sysctl |= VME_BUS_ARB_PRRS;
            break;
        case VME_BUS_ARB_PRI:
            *sysctl &= VME_BUS_ARB_MASK;
            *sysctl |= VME_BUS_ARB_PRI;
            break;
        default:
            // Someone passed in something dumb,
            // lets default to round robin
            *sysctl &= VME_BUS_ARB_MASK;
            *sysctl |= VME_BUS_ARB_RRS;
            break;
    } 
}

/*
 *   VME_BUS_TIMEOUT_16US    = 0 << 6,
 *   VME_BUS_TIMEOUT_32US    = 1 << 6,
 *   VME_BUS_TIMEOUT_64US    = 2 << 6,
 *   VME_BUS_TIMEOUT_128US   = 3 << 6,
 *   VME_BUS_TIMEOUT_MASK    = 0x3f,
 */

void BoardSetVMEBusTimeout(sysctl_bits_t method)
{
    switch (method) {
        case VME_BUS_TIMEOUT_16US:
            *sysctl &= VME_BUS_TIMEOUT_MASK;
            *sysctl |= VME_BUS_TIMEOUT_16US;
            break;

        case VME_BUS_TIMEOUT_32US:
            *sysctl &= VME_BUS_TIMEOUT_MASK;
            *sysctl |= VME_BUS_TIMEOUT_32US;
            break;

        case VME_BUS_TIMEOUT_64US:
            *sysctl &= VME_BUS_TIMEOUT_MASK;
            *sysctl |= VME_BUS_TIMEOUT_64US;
            break;

        case VME_BUS_TIMEOUT_128US:
            *sysctl &= VME_BUS_TIMEOUT_MASK;
            *sysctl |= VME_BUS_TIMEOUT_128US;
            break;

        default:
            // Someone passed in something dumb,
            // lets default to 128us.
            *sysctl &= VME_BUS_TIMEOUT_MASK;
            *sysctl |= VME_BUS_TIMEOUT_128US;
            break;
    } 
}
/**
 * initMSM: Set up the millisecond marker timer
 *
 */
void BoardInitMSM(irq_handler_t handler, irq_handler_t vec, uint8_t lvl) // This sets the MSM to 1ms interrupts, we could use 10 I suppose but 1 and 10 are the only choices...
{
    uint8_t *msmctl     = (uint8_t *)(0xfff90051);
    uint8_t *icmsm      = (uint8_t *)(0xfff9004f);
    uint8_t *vectmsm    = (uint8_t *)(0xfff90019);
    uint8_t *irmaskb    = (uint8_t *)(0xfff9001f);

    *msmctl = 0x03; // Set freq to 1ms
    if (lvl > 7) lvl = 7;
    *icmsm = lvl; // Set interrupt level, between 1-7, 0 is disable.
    *vectmsm = (uint32_t)(vec)/4;  // This is very unsafe!  It relies on pointers being 32bits.
    *(uint32_t*)(vec) = (uint32_t)(handler);
    *irmaskb |= 0x40; // unmask MSM irq.
}