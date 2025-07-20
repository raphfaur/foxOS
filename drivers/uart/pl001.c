#include "pl001.h"
#include "../../kernel/utils/debug.h"

struct serial UART = {.base_address = (const char *)0x9000000};


#define UARTIMSC 0x038 
#define UARTRIS 0x03C 
#define UARTMIS 0x040
#define UARTICR 0x044
#define UARTCR

extern char _debug_heap;
extern char _heap_start2;
extern char _heap_start3;

#define DEBUG_SPACE1 &_debug_heap
#define DEBUG_SPACE2 &_heap_start2
#define DEBUG_SPACE3 &_heap_start3


void pl011_init(){
  *((volatile uint16_t *) (UART.base_address + UARTIMSC)) |= (1 << 4);
}

char pl011_read_char(struct serial *dev) {
  volatile char *cursor = (volatile char *)dev->base_address;
  return *cursor;
}

void pl011_send(struct serial *dev, const char *data) {
  char *data_cursor = (char *)data;
  volatile char *cursor = (volatile char *)dev->base_address;
  while (*data_cursor != 0) {
    *cursor = *data_cursor;
    data_cursor++;
  }
  *((volatile uint16_t *) (UART.base_address + UARTICR)) |= (1 << 5);
}

void pl011_send_int(struct serial *dev, const unsigned long long n, size_t digits, int base) {
  unsigned long q = n;
  *(DEBUG_SPACE1 + digits ) = 0;
  for (int i=1; i<=digits; i++) {
    int r = q % base;
    q /= base;
    char c;
    if (r >= 10) {
      c = (char) (r - 10) + 'a';
    } else {
      c = (char) r + '0';
    }
    *(DEBUG_SPACE1 + digits - i) = c;
  }
  pl011_send(dev, DEBUG_SPACE1);
}

