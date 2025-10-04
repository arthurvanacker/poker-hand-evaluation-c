/*
 * Allocation Failure Test Suite
 * Tests behavior when malloc/calloc fail (return NULL)
 *
 * This test file verifies graceful handling of memory allocation failures.
 * Uses malloc interposition via LD_PRELOAD to simulate allocation failures
 * without modifying production code.
 */

#define _GNU_SOURCE
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include "../include/poker.h"

/*
 * Get pointer to fail_next_malloc variable from malloc_wrapper.so
 * This variable is defined in malloc_wrapper.so and controls failure simulation
 */
static int* get_fail_next_malloc_ptr(void) {
    static int* ptr = NULL;
    if (ptr == NULL) {
        /* RTLD_DEFAULT searches all loaded libraries */
        ptr = (int*)dlsym(RTLD_DEFAULT, "fail_next_malloc");
        if (ptr == NULL) {
            fprintf(stderr, "ERROR: Could not find fail_next_malloc symbol.\n");
            fprintf(stderr, "Make sure to run with: LD_PRELOAD=./tests/malloc_wrapper.so\n");
            exit(1);
        }
    }
    return ptr;
}

/* Macro for setting fail_next_malloc */
#define SET_FAIL_COUNT(n) (*get_fail_next_malloc_ptr() = (n))

/**
 * Test: deck_new() returns NULL when Deck struct allocation fails
 *
 * Simulates malloc failure for Deck struct allocation (first malloc in deck_new).
 * Expected behavior: deck_new() returns NULL without crashing.
 */
void test_deck_new_fails_on_struct_allocation(void) {
    printf("Testing deck_new() failure when Deck struct allocation fails...\n");

    // Simulate failure of first malloc (Deck struct)
    SET_FAIL_COUNT(1);

    Deck* deck = deck_new();

    // Verify deck_new returns NULL on allocation failure
    assert(deck == NULL);

    // Verify poker_errno is set to POKER_ENOMEM
    assert(poker_errno == POKER_ENOMEM);

    printf("  ✓ deck_new() returns NULL when Deck struct allocation fails\n");
}

/**
 * Test: deck_new() returns NULL when cards array allocation fails
 *
 * Simulates malloc failure for cards array allocation (second malloc in deck_new).
 * Expected behavior: deck_new() returns NULL and frees Deck struct (no leak).
 */
void test_deck_new_fails_on_cards_allocation(void) {
    printf("Testing deck_new() failure when cards array allocation fails...\n");

    // Skip first malloc (Deck struct succeeds), fail second malloc (cards array)
    SET_FAIL_COUNT(2);  // Fail on 2nd malloc call

    Deck* deck = deck_new();

    // Verify deck_new returns NULL on allocation failure
    assert(deck == NULL);

    // Verify poker_errno is set to POKER_ENOMEM
    assert(poker_errno == POKER_ENOMEM);

    printf("  ✓ deck_new() returns NULL when cards array allocation fails\n");
}

/**
 * Test: deck_new() succeeds when allocations work
 *
 * Baseline test - no simulated failures. Verifies test infrastructure
 * doesn't interfere with normal operation.
 */
void test_deck_new_succeeds_normally(void) {
    printf("Testing deck_new() succeeds when allocations work...\n");

    SET_FAIL_COUNT(0);  // No failures

    Deck* deck = deck_new();

    // Verify deck creation succeeded
    assert(deck != NULL);
    assert(deck->cards != NULL);
    assert(deck->size == DECK_SIZE);

    deck_free(deck);

    printf("  ✓ deck_new() succeeds when allocations work normally\n");
}

int main(void) {
    printf("\n=== Allocation Failure Test Suite ===\n\n");

    test_deck_new_fails_on_struct_allocation();
    test_deck_new_fails_on_cards_allocation();
    test_deck_new_succeeds_normally();

    printf("\n=== All allocation failure tests passed! ===\n\n");
    return 0;
}
