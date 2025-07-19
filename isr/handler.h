#include <stdint.h>

typedef void (* isr_routine )();

/* Register interrupt routine */
void __register_routine_spi(uint32_t spi, isr_routine routine);
void __register_routine_ppi(uint32_t ppi, isr_routine routine);
void __register_routine_sgi(uint32_t sgi, isr_routine routine);