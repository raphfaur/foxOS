#include <stdint.h>
#include <stddef.h>

extern char _kernel_heap_start;
extern char _kernel_heap_end;
extern int _kernel_heap_physical;
/* CONFIG */

/* 4KB page size */
#define PAGE_SIZE (64 * pow(2, 10))
#define PAGE_N ( 0x20000000 >> 16 )
#define ALLOCATOR_BASE (&_kernel_heap_start)
#define MAX_ORDER (13)
#define BT_DEPTH MAX_ORDER

/* 2^(order + 1) - 1 */
#define BLOCK_COUNT ( 16383 )

/* DEFS */
#define BLOCK_FREE 0
#define BLOCK_ALLOCATED 1

#define BLOCK_SPLITTED 0
#define BLOCK_MERGED 1

struct block;

typedef struct block {
    /* Block order is BT_DEPTH - depth */
    /* Block size is 2^order * PAGE_SIZE */
    char order;
    int allocated;
    int split;
    struct block *left;
    struct block *right;
    struct block *parent;
    size_t index;
} block_t;

struct address_w {
    void * address;
    int valid;
};

void init_allocator();
void * kalloc(size_t size);
void kfree(void *address);