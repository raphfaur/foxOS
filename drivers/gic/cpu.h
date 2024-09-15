#include <stdint.h>

struct gic_cpu {
  void * base_address;
};
void gic_cpu_init(struct gic_cpu* dev);
uint32_t get_ICC_SRE_EL1();
uint32_t get_ICC_IGRPEN1_EL1();

int get_intid_mode_0();


struct gic_distributor {
  void * base_address;
};
void set_EnableGrp1S(struct gic_distributor * dev, int i );
void gic_distributor_init(struct gic_distributor* dev);
uint32_t get_GICD_CTLR(struct gic_distributor *dev);


struct gic_redistributor {
  void * base_address;
};
void gic_redistributor_init(struct gic_redistributor* dev);
uint32_t get_GICR_WAKER(struct gic_redistributor *dev);
uint32_t get_GICR_IIDR();

uint32_t get_periphbase();

void try(struct gic_distributor *dev);
uint32_t clear_gicc_interrupt();
void clear_gicc_interrupt_id(uint32_t intid);

// Routing
uint64_t read_SCR_EL3();

