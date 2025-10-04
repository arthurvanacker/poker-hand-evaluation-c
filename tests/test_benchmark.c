/*
 * Tests for benchmark infrastructure
 * Tests timer accuracy and benchmark result formatting
 */

#define _POSIX_C_SOURCE 199309L  /* Required for clock_gettime */

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "../include/poker.h"

/* Unity test framework */
void setUp(void) {}
void tearDown(void) {}

/*
 * Test that clock_gettime works with CLOCK_MONOTONIC
 */
void test_clock_gettime_works(void) {
    struct timespec start, end;
    int ret;

    /* Get start time */
    ret = clock_gettime(CLOCK_MONOTONIC, &start);
    if (ret != 0) {
        printf("FAIL: clock_gettime failed\n");
        return;
    }

    /* Sleep for a known duration */
    usleep(10000); /* 10ms = 0.01 seconds */

    /* Get end time */
    ret = clock_gettime(CLOCK_MONOTONIC, &end);
    if (ret != 0) {
        printf("FAIL: clock_gettime failed\n");
        return;
    }

    /* Calculate elapsed time */
    double elapsed = (end.tv_sec - start.tv_sec) +
                     (end.tv_nsec - start.tv_nsec) / 1e9;

    /* Should be approximately 0.01 seconds (with some tolerance) */
    if (elapsed >= 0.008 && elapsed <= 0.020) {
        printf("PASS: test_clock_gettime_works\n");
    } else {
        printf("FAIL: test_clock_gettime_works - elapsed=%f (expected ~0.01)\n", elapsed);
    }
}

/*
 * Test that we can measure operations per second
 */
void test_benchmark_basic_loop(void) {
    struct timespec start, end;
    int iterations = 0;
    int i;

    /* Run loop for at least 0.01 seconds */
    clock_gettime(CLOCK_MONOTONIC, &start);
    do {
        for (i = 0; i < 10000; i++) {
            iterations++;
        }
        clock_gettime(CLOCK_MONOTONIC, &end);
    } while ((end.tv_sec - start.tv_sec) +
             (end.tv_nsec - start.tv_nsec) / 1e9 < 0.01);

    double elapsed = (end.tv_sec - start.tv_sec) +
                     (end.tv_nsec - start.tv_nsec) / 1e9;
    double ops_per_sec = iterations / elapsed;

    /* Should have executed at least 10,000 iterations */
    if (iterations >= 10000 && ops_per_sec > 0) {
        printf("PASS: test_benchmark_basic_loop - %d iterations, %.0f ops/sec\n",
               iterations, ops_per_sec);
    } else {
        printf("FAIL: test_benchmark_basic_loop - iterations=%d, ops_per_sec=%.0f\n",
               iterations, ops_per_sec);
    }
}

/*
 * Format a large number with thousand separators
 * Example: 1234567 -> "1,234,567"
 */
void format_number(long num, char* buffer, size_t size) {
    char temp[64];
    int len, i, j, comma_count;

    if (buffer == NULL || size == 0) {
        return;
    }

    /* Handle negative numbers */
    int is_negative = 0;
    if (num < 0) {
        is_negative = 1;
        num = -num;
    }

    /* Convert to string */
    snprintf(temp, sizeof(temp), "%ld", num);
    len = (int)strlen(temp);

    /* Calculate number of commas needed */
    comma_count = (len - 1) / 3;

    /* Check buffer size */
    if ((size_t)(len + comma_count + (is_negative ? 1 : 0) + 1) > size) {
        buffer[0] = '\0';
        return;
    }

    /* Build formatted string from right to left */
    i = len - 1;  /* Index in temp */
    j = len + comma_count + (is_negative ? 1 : 0);  /* Index in buffer */
    buffer[j] = '\0';
    j--;

    int digit_count = 0;
    while (i >= 0) {
        if (digit_count == 3) {
            buffer[j--] = ',';
            digit_count = 0;
        }
        buffer[j--] = temp[i--];
        digit_count++;
    }

    if (is_negative) {
        buffer[j] = '-';
    }
}

/*
 * Test formatting large numbers with commas
 */
void test_format_number_with_commas(void) {
    char buffer[64];

    /* Test basic formatting */
    format_number(1234567, buffer, sizeof(buffer));
    if (strcmp(buffer, "1,234,567") == 0) {
        printf("PASS: test_format_number_with_commas - 1234567 -> %s\n", buffer);
    } else {
        printf("FAIL: test_format_number_with_commas - expected '1,234,567', got '%s'\n", buffer);
        return;
    }

    /* Test smaller number */
    format_number(999, buffer, sizeof(buffer));
    if (strcmp(buffer, "999") == 0) {
        printf("PASS: test_format_number_with_commas - 999 -> %s\n", buffer);
    } else {
        printf("FAIL: test_format_number_with_commas - expected '999', got '%s'\n", buffer);
    }
}

int main(void) {
    printf("\n=== Benchmark Infrastructure Tests ===\n\n");

    test_clock_gettime_works();
    test_benchmark_basic_loop();
    test_format_number_with_commas();

    printf("\n=== All benchmark infrastructure tests completed ===\n");
    return 0;
}
