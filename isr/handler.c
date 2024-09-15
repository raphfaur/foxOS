#include "../drivers/gic/cpu.h"
#include "../utils/debug.h"

void __fiq_bridge(uint64_t id) {
    DEBUGH(id);
    DEBUG("Handling SMC");
}