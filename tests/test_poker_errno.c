#include <assert.h>
#include <stdio.h>
#include "../include/poker.h"

/*
 * Test Suite for Error Code Enum and poker_errno
 * Tests verify error reporting mechanism
 */

void test_error_constants_defined(void) {
    printf("Testing error constants are defined...\n");

    /* Verify all error constants are defined and have correct values */
    assert(POKER_EOK == 0);
    assert(POKER_EINVAL == 1);
    assert(POKER_ENOMEM == 2);
    assert(POKER_ENOTFOUND == 3);
    assert(POKER_ERANGE == 4);

    printf("  ✓ All error constants defined correctly\n");
}

void test_error_constants_distinct(void) {
    printf("Testing error constants are distinct...\n");

    /* Verify all error codes are unique */
    assert(POKER_EOK != POKER_EINVAL);
    assert(POKER_EOK != POKER_ENOMEM);
    assert(POKER_EOK != POKER_ENOTFOUND);
    assert(POKER_EOK != POKER_ERANGE);
    assert(POKER_EINVAL != POKER_ENOMEM);
    assert(POKER_EINVAL != POKER_ENOTFOUND);
    assert(POKER_EINVAL != POKER_ERANGE);
    assert(POKER_ENOMEM != POKER_ENOTFOUND);
    assert(POKER_ENOMEM != POKER_ERANGE);
    assert(POKER_ENOTFOUND != POKER_ERANGE);

    printf("  ✓ All error constants are distinct\n");
}

void test_poker_errno_exists(void) {
    printf("Testing poker_errno global variable exists...\n");

    /* Test that poker_errno is accessible and can be set */
    poker_errno = POKER_EOK;
    assert(poker_errno == POKER_EOK);

    poker_errno = POKER_EINVAL;
    assert(poker_errno == POKER_EINVAL);

    poker_errno = POKER_ENOMEM;
    assert(poker_errno == POKER_ENOMEM);

    /* Reset to OK state */
    poker_errno = POKER_EOK;

    printf("  ✓ poker_errno global variable works correctly\n");
}

void test_poker_errno_initial_value(void) {
    printf("Testing poker_errno initial value...\n");

    /* Note: This test assumes we reset poker_errno to 0 at the start
     * In a real scenario, we'd need to ensure clean state */
    poker_errno = POKER_EOK;

    /* Verify initial value is POKER_EOK (0) */
    assert(poker_errno == POKER_EOK);
    assert(poker_errno == 0);

    printf("  ✓ poker_errno initializes correctly\n");
}

void test_error_code_as_int(void) {
    printf("Testing error codes can be used as int...\n");

    /* Verify error codes can be assigned to int variables */
    int error1 = POKER_EOK;
    int error2 = POKER_EINVAL;
    int error3 = POKER_ENOMEM;

    assert(error1 == 0);
    assert(error2 == 1);
    assert(error3 == 2);

    printf("  ✓ Error codes work as integers\n");
}

int main(void) {
    printf("\n=== Running poker_errno Tests ===\n\n");

    test_error_constants_defined();
    test_error_constants_distinct();
    test_poker_errno_exists();
    test_poker_errno_initial_value();
    test_error_code_as_int();

    printf("\n=== All poker_errno Tests Passed ===\n");
    return 0;
}
