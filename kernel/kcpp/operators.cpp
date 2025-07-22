#include <new>
#include <stdlib.h>

void operator delete(void *, unsigned long) {
    
}

void operator delete(void *) {
    
}

extern "C" void * kalloc(std::size_t size);

void * operator new(std::size_t size) {
    return malloc(size);
}

namespace std {
    void terminate() noexcept;
}