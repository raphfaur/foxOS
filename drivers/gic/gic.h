#ifndef __GIC
#define __GIC
#include <stdint.h>
#include "../../utils/debug.h"

extern char _vbar_address;
#define REDISTRIBUTOR_BASE_ADDRESS 0x80a0000
#define DISTRIBUTOR_BASE_ADDRESS 0x8000000

#define GICD_CTLR 0x0000
#define GICD_IGROUPR 0x0080
#define GICR_WAKER 0x0014


#define GICR_SGI_BASE 0x10000
#define GICR_IGROUP 0x0080
#define GICR_ISENABLER 0x0100
#define GICR_ICFGR1 0x0c04
#define GICR_ICFGR0 0x0c00

#define GICC_PMR 0x0004

#define GIC_MODE_LEVEL_SENSITIVE (0b00ULL)
#define GIC_MODE_EDGE_TRIGGERRED (0b10ULL)


/*
https://developer.arm.com/documentation/198123/0302/Arm-GIC-fundamentals
*/

/* Global methods */
void _gic_init();

/*
 * Interrupt ID (INTID) Ranges and Types — GICv3 / GICv3.1
 * -------------------------------------------------------
 *
 * +-----------------------------+------------------+-------------------------------------------+
 * | INTID Range                 | Interrupt Type   | Notes                                     |
 * +-----------------------------+------------------+-------------------------------------------+
 * | 0 - 15                      | SGIs             | Banked per PE (Software Generated IRQs)   |
 * | 16 - 31                     | PPIs             | Banked per PE (Private Peripheral IRQs)   |
 * | 32 - 1019                   | SPIs             | Shared Peripheral IRQs                    |
 * | 1020 - 1023                 | Special INTIDs   | Used for special signals per PE           |
 * | 1024 - 1055                 | Reserved         | –                                         |
 * | 1056 - 1119 (GICv3.1 only)  | PPIs             | Extended PPIs (banked per PE)             |
 * | 1120 - 4095                 | Reserved         | –                                         |
 * | 4096 - 5119 (GICv3.1 only)  | SPIs             | Extended SPIs                             |
 * | 5120 - 8191                 | Reserved         | –                                         |
 * | 8192 and greater            | LPIs             | Locality-specific (IMPLEMENTATION DEFINED)|
 * +-----------------------------+------------------+-------------------------------------------+
 *
*/
void _gic_register_spi(int spi, uint64_t affinity, uint8_t mode);
void _gic_register_ppi(int ppi, uint8_t mode);
void _gic_register_sgi(int sgi, uint64_t affinity, uint8_t mode);



/*
 *
 * +-------------------------+------------------+------------------+------------------+
 * | EL and Security State   | Group 0          | Group 1 Secure   | Group 1 Non-sec  |
 * +-------------------------+------------------+------------------+------------------+
 * | Secure EL0/1            | FIQ              | IRQ              | FIQ              |
 * | Non-secure EL0/1/2      | -                | FIQ              | IRQ              |
 * | EL3                     | FIQ              | FIQ              | FIQ              |
 * +-------------------------+------------------+------------------+------------------+
 *
*/
// CPU Interface
struct gic_cpu {
  void * base_address;
};
void gic_cpu_init(struct gic_cpu* dev);
uint32_t get_ICC_SRE_EL1();
uint32_t get_ICC_IGRPEN1_EL1();

int get_intid_mode_0();


// Distributor
struct gic_distributor {
  void * base_address;
};
void gic_distributor_init(struct gic_distributor* dev);

void set_mode_for_intid_d(struct gic_distributor *dev, int intid, uint8_t mode);
void route_intid_d(struct gic_distributor *dev, int intid, uint64_t affinity);
void enable_intid_d(struct gic_distributor *dev, int intid);


// Redistributor
struct gic_redistributor {
  void * base_address;
};
void gic_redistributor_init(struct gic_redistributor* dev);
void set_mode_for_intid_r(struct gic_redistributor *dev, int intid, uint8_t mode);
void enable_intid_r(struct gic_redistributor *dev, int intid);



// Interrupt handling
uint32_t read_group0_intid();
uint32_t read_group1_intid();
void end_of_interrupt_group0(uint32_t intid);
void end_of_interrupt_group1(uint32_t intid);

#endif