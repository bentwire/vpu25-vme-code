#ifndef __VPU25_H__
#define __VPU25_H__

typedef enum {
    VME_BUS_TIMEOUT_16US    = 0 << 6,
    VME_BUS_TIMEOUT_32US    = 1 << 6,
    VME_BUS_TIMEOUT_64US    = 2 << 6,
    VME_BUS_TIMEOUT_128US   = 3 << 6,
    VME_BUS_TIMEOUT_MASK    = 0x3f,
    VME_BUS_ARB_RRS         = 1 << 4,
    VME_BUS_ARB_PRRS        = 2 << 4,
    VME_BUS_ARB_PRI         = 3 << 4,
    VME_BUS_ARB_MASK        = 0xcf,
    VME_BUS_BR_ROR          = 0 << 1,
    VME_BUS_BR_RWD          = 1 << 1,
    VME_BUS_BR_MASK         = 0xfd,
} sysctl_bits_t;

typedef void (*irq_handler_t)(uint16_t vect);

void BoardSetVMEBusReqLvl(uint8_t lvl);
void BoardSetVMEBusRelMethod(sysctl_bits_t method);
void BoardSetVMEBusArbMethod(sysctl_bits_t method);
void BoardSetVMEBusTimeout(sysctl_bits_t method);
void BoardInitMSM(irq_handler_t handler, irq_handler_t vec, uint8_t lvl); // This sets the MSM to 1ms interrupts, we could use 10 I suppose but 1 and 10 are the only choices...

#endif //__VPU25_H__
