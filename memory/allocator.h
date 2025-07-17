#include <stdint.h>
#include <stddef.h>

#define MAX_ORDER 4
#define BT_DEPTH MAX_ORDER

/* 64KB page size */
#define PAGE_SIZE (64 * pow(2, 10))

struct block;

struct block {
    /* Block order is BT_DEPTH - depth */
    /* Block size is 2^order * PAGE_SIZE */
    char order;
    int allocated;
    int split;
    struct block *left;
    struct block *right;
    struct block *parent;
    size_t index;
};

struct address_w {
    void * address;
    int valid;
};

typedef struct block block_t;

void * alloc(int order);
int free(void * address);
void init_allocator();

