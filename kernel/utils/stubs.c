
void *memcpy(void *dest, const void *src, unsigned long n) {
    unsigned char *d = dest;
    const unsigned char *s = src;
    while (n--) {
        *d++ = *s++;
    }
    return dest;
}

int memcmp(const void *s1, const void *s2, unsigned long n) {
    const unsigned char *p1 = s1;
    const unsigned char *p2 = s2;
    while (n--) {
        if (*p1 != *p2)
            return *p1 - *p2;
        p1++;
        p2++;
    }
    return 0;
}

int __errno_var;

int *__errno(void) {
    return &__errno_var;
}


