/*
 * malloc_wrapper.c - Malloc interposition library for testing allocation failures
 *
 * This library intercepts malloc calls via LD_PRELOAD to simulate allocation
 * failures in tests. When fail_next_malloc > 0, the next N malloc calls will
 * return NULL instead of allocating memory.
 *
 * Compilation:
 *   gcc -shared -fPIC -o malloc_wrapper.so malloc_wrapper.c -ldl
 *
 * Usage:
 *   LD_PRELOAD=./malloc_wrapper.so ./test_allocation_failure
 */

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

/* Control variable - defined here, accessible from test via symbol lookup */
int fail_next_malloc = 0;

/* Function pointer to real malloc */
static void* (*real_malloc)(size_t) = NULL;

/**
 * Initialize real_malloc function pointer on first use
 */
static void malloc_wrapper_init(void) {
    if (real_malloc == NULL) {
        real_malloc = dlsym(RTLD_NEXT, "malloc");
        if (real_malloc == NULL) {
            fprintf(stderr, "malloc_wrapper: failed to load real malloc\n");
            exit(1);
        }
    }
}

/**
 * malloc() interposition function
 *
 * Intercepts all malloc calls and returns NULL if fail_next_malloc > 0.
 * Decrements fail_next_malloc counter and forwards to real malloc otherwise.
 */
void* malloc(size_t size) {
    malloc_wrapper_init();

    /* Check if we should fail this allocation */
    if (fail_next_malloc > 0) {
        fail_next_malloc--;
        fprintf(stderr, "malloc_wrapper: simulating allocation failure (remaining: %d)\n",
                fail_next_malloc);
        return NULL;
    }

    /* Forward to real malloc */
    return real_malloc(size);
}
