#include "gic.h"

struct gic_distributor gicd;
struct gic_redistributor gicr;
struct gic_cpu gicc;

void _gic_init() {
    DEBUG("Initializing GIC")

    DEBUG("Initializing GICD")
    gic_distributor_init(&gicd);

    DEBUG("Initiqlizing GICR")
    gic_redistributor_init(&gicr);

    DEBUG("Initiqlizing CPU Interface")
    gic_cpu_init(&gicc);
}

/* Shared Peripheral Interrupts */
void _gic_register_spi(int spi, uint64_t affinity, uint8_t mode) {
    DEBUG("Registering new SPI")
    enable_intid_d(&gicd, spi + 32);
    route_intid_d(&gicd, spi + 32, affinity);
    set_mode_for_intid_d(&gicd, spi + 32, mode);
}

/* Private Peripheral Interrupts */
void _gic_register_ppi(int ppi, uint8_t mode) {
    DEBUG("Registering new PPI")
    enable_intid_r(&gicr, ppi + 16);
    set_mode_for_intid_r(&gicr, ppi + 16, mode);
}

/* Software Generated Interrupts */
void _gic_register_sgi(int sgi, uint64_t affinity, uint8_t mode) {
    DEBUG("Registering new SGI")
    enable_intid_r(&gicr, sgi);
    set_mode_for_intid_r(&gicr, sgi, mode);
}
