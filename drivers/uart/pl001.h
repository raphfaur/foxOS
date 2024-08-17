#include <stdint.h>  // needed for uint32_t type
#include <stddef.h>

#ifndef PL001
#define PL001

struct serial {
  const char * base_address;
};

void pl011_send(struct serial*, const char *);

void pl011_send_int(struct serial *dev, const unsigned long long n, size_t digits, int base);
#endif
