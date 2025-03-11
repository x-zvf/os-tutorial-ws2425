#include <stdint.h>
#include <stdio.h>

#define FIELDS \
    X(int32_t, a) \
    X(char, b) \
    X(int,x) \
    X(int,y) \
    X(short, d)

struct mystruct {
#define X(type, name) type name;
    FIELDS
#undef X
} S;

#define PRINT_PADDING(offset, padding) \
    if(padding) \
        printf("  /* %2lu */          /* padding */        // %lu\n", offset, padding);
#define PRINT_VAR(offset, type, name) \
    printf("  /* %2lu */     %-10s %10s;    // %lu\n", offset, #type, #name, sizeof(S.name));

int main(void) {
    intptr_t last_offset = 0;
    intptr_t last_size = 0;
    intptr_t offset, padding, padding_start;
    printf("// offset      type       name              size\n");
    printf("               struct     mystruct {     // %lu\n\n", sizeof(S));

#define X(type, name)\
    offset = (intptr_t)(&S.name) - (intptr_t)(&S); \
    padding_start = last_offset + last_size; \
    padding = offset - padding_start; \
    last_offset = (intptr_t)(&S.name) - (intptr_t)(&S); \
    last_size = sizeof(S.name);\
    \
    PRINT_PADDING(padding_start, padding) \
    PRINT_VAR(offset, type, name);

    FIELDS
#undef X
    PRINT_PADDING(last_offset + last_size, sizeof(S) - last_offset - last_size);
    printf("};\n");
}

