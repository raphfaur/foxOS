#include "pl001.h"

struct serial UART = {.base_address = (const char *)0x9000000};

extern char _heap_start1;
extern char _heap_start2;
extern char _heap_start3;

#define DEBUG_SPACE1 &_heap_start1
#define DEBUG_SPACE2 &_heap_start2
#define DEBUG_SPACE3 &_heap_start3

void pl011_send(struct serial *dev, const char *data) {
  char *data_cursor = (char *)data;
  volatile char *cursor = (volatile char *)dev->base_address;
  while (*data_cursor != 0) {
    *cursor = *data_cursor;
    data_cursor++;
  }
}

void pl011_send_int(struct serial *dev, const unsigned long long n, size_t digits, int base) {
  unsigned long q = n;
  *(DEBUG_SPACE1 + digits ) = 0;
  for (int i=1; i<=digits; i++) {
    int r = q % base;
    q /= base;
    char c = (char) r + '0';
    *(DEBUG_SPACE1 + digits - i) = c;
  }
  pl011_send(dev, DEBUG_SPACE1);
}
