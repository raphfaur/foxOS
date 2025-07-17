#include <stddef.h>

#define MAX_PROCESS_MEMORY_SPACE_N 3
// 8192 * 8
#define BLOCK_N (8192 * 8)

struct process_block;

struct process_block {
    size_t index;
    size_t size;
    int process_id;
    struct process_block * next_block;
};

void debug_process_memory(int process_id);
void * process_alloc(int order, int process_id);
struct process_block* init_process_memory(int process_id);
void process_dealloc(void *address, int process_id);
