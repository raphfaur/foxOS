#include <stdint.h>
#include <stddef.h>
#include <sys/types.h>
#include "../utils/debug.h"

int _close(int file) {
  return -1;
}

int _lseek(int file, int ptr, int dir) {
  return 0;
}


int _write(int file, char *ptr, int len) {
    return -1;
}

int _read(int file, char *ptr, int len) {
  return -1;
}

extern char _kernel_heap_start;
extern char _kernel_heap_end;
uint8_t * cursor = &_kernel_heap_start;
uint8_t * end = &_kernel_heap_end;

caddr_t _sbrk(int incr) {
  if (cursor + incr < end) {
    cursor += incr;
  }
  DEBUGH((uint64_t) cursor);
  return cursor;
}