#ifndef utils 
#define utils 


#include "../../drivers/uart/pl001.h"

extern struct serial UART;

#define DEBUGB(value) \
pl011_send(&UART, __FILE__ ":"); \
pl011_send_int(&UART, __LINE__, 3, 10); \
pl011_send(&UART, " | " #value "=0b"); \
pl011_send_int(&UART, value, sizeof(value) * 8, 2 ); \
pl011_send(&UART, "\n" );

#define DEBUGH(value) \
pl011_send(&UART, __FILE__ ":"); \
pl011_send_int(&UART, __LINE__, 3, 10); \
pl011_send(&UART, " | " #value "=0x"); \
pl011_send_int(&UART, value, sizeof(value) * 2, 16 ); \
pl011_send(&UART, "\n" );

#define DEBUGD(value) \
pl011_send(&UART, __FILE__ ":"); \
pl011_send_int(&UART, __LINE__, 3, 10); \
pl011_send(&UART, " | " #value "="); \
pl011_send_int(&UART, value, 10, 10 ); \
pl011_send(&UART, "\n" );

#define DEBUG(value) \
pl011_send(&UART, value "\n");

uint64_t get_current_stack_pointer() ;

int atoi(char *);

#endif 
