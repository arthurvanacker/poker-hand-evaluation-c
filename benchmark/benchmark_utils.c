/*
 * Benchmark utilities implementation
 * Formatting and output functions for benchmark results
 */

#include "benchmark.h"
#include <stdio.h>
#include <string.h>

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
 * Print benchmark results in table format
 */
void print_benchmark_table(const BenchmarkResult* results, size_t count) {
    size_t i;
    char formatted[64];

    printf("\nPoker Hand Evaluator Benchmarks\n");
    printf("================================\n");

    for (i = 0; i < count; i++) {
        format_number((long)results[i].ops_per_sec, formatted, sizeof(formatted));
        printf("%-30s %15s ops/sec\n", results[i].name, formatted);
    }

    printf("\n");
}
