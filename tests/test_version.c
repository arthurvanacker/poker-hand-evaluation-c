/*
 * Test suite for API version macros
 * Verifies version tracking and compatibility check macros
 */

#include "../include/poker.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

/* Test that version major macro exists and has correct value */
static void test_version_major_defined(void) {
    /* POKER_VERSION_MAJOR should be 0 for initial release */
    assert(POKER_VERSION_MAJOR == 0);

    printf("test_version_major_defined: PASSED\n");
}

/* Test that version minor macro exists and has correct value */
static void test_version_minor_defined(void) {
    /* POKER_VERSION_MINOR should be 3 for this release */
    assert(POKER_VERSION_MINOR == 3);

    printf("test_version_minor_defined: PASSED\n");
}

/* Test that version patch macro exists and has correct value */
static void test_version_patch_defined(void) {
    /* POKER_VERSION_PATCH should be 0 for this release */
    assert(POKER_VERSION_PATCH == 0);

    printf("test_version_patch_defined: PASSED\n");
}

/* Test that version string macro exists and has correct format */
static void test_version_string_defined(void) {
    /* POKER_VERSION should be "0.3.0" */
    assert(strcmp(POKER_VERSION, "0.3.0") == 0);

    printf("test_version_string_defined: PASSED\n");
}

/* Test that version check macro exists and calculates correctly */
static void test_version_check_macro(void) {
    /* POKER_VERSION_CHECK should calculate: major*10000 + minor*100 + patch */
    int v030 = POKER_VERSION_CHECK(0, 3, 0);
    assert(v030 == 300);  /* 0*10000 + 3*100 + 0 = 300 */

    int v100 = POKER_VERSION_CHECK(1, 0, 0);
    assert(v100 == 10000);  /* 1*10000 + 0*100 + 0 = 10000 */

    int v123 = POKER_VERSION_CHECK(1, 2, 3);
    assert(v123 == 10203);  /* 1*10000 + 2*100 + 3 = 10203 */

    printf("test_version_check_macro: PASSED\n");
}

/* Test that version number macro exists and has correct value */
static void test_version_number_defined(void) {
    /* POKER_VERSION_NUMBER should match current version */
    assert(POKER_VERSION_NUMBER == POKER_VERSION_CHECK(0, 3, 0));
    assert(POKER_VERSION_NUMBER == 300);

    printf("test_version_number_defined: PASSED\n");
}

/* Test version comparison for compile-time compatibility checks */
static void test_version_comparison(void) {
    /* Test that current version is >= 0.3.0 */
    assert(POKER_VERSION_NUMBER >= POKER_VERSION_CHECK(0, 3, 0));

    /* Test that current version is < 1.0.0 */
    assert(POKER_VERSION_NUMBER < POKER_VERSION_CHECK(1, 0, 0));

    /* Test ordering */
    assert(POKER_VERSION_CHECK(0, 2, 0) < POKER_VERSION_NUMBER);
    assert(POKER_VERSION_CHECK(0, 4, 0) > POKER_VERSION_NUMBER);

    printf("test_version_comparison: PASSED\n");
}

/* Test version macro for conditional compilation */
static void test_conditional_compilation(void) {
    int feature_available = 0;

    /* Simulate conditional feature based on version */
#if POKER_VERSION_NUMBER >= POKER_VERSION_CHECK(0, 3, 0)
    feature_available = 1;
#endif

    assert(feature_available == 1);

    printf("test_conditional_compilation: PASSED\n");
}

int main(void) {
    printf("Running version macro tests...\n\n");

    test_version_major_defined();
    test_version_minor_defined();
    test_version_patch_defined();
    test_version_string_defined();
    test_version_check_macro();
    test_version_number_defined();
    test_version_comparison();
    test_conditional_compilation();

    printf("\nAll version macro tests passed!\n");
    return 0;
}
