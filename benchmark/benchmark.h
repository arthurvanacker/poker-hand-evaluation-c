/*
 * Benchmark infrastructure header
 * High-resolution timing and performance measurement utilities
 */

#ifndef BENCHMARK_H
#define BENCHMARK_H

#define _POSIX_C_SOURCE 199309L  /* Required for clock_gettime */

#include <time.h>
#include <stddef.h>

/*
 * Benchmark result structure
 */
typedef struct {
    const char* name;        /* Benchmark name */
    double ops_per_sec;      /* Operations per second */
    int iterations;          /* Total iterations executed */
    double elapsed_sec;      /* Total elapsed time in seconds */
} BenchmarkResult;

/*
 * Format a large number with thousand separators
 * Example: 1234567 -> "1,234,567"
 */
void format_number(long num, char* buffer, size_t size);

/*
 * Print benchmark results in table format
 */
void print_benchmark_table(const BenchmarkResult* results, size_t count);

#endif /* BENCHMARK_H */
