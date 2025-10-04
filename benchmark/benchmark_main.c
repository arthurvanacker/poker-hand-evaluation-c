/*
 * Main benchmark runner
 * Runs all benchmarks and displays results in table format
 */

#define _POSIX_C_SOURCE 199309L

#include <stdio.h>
#include "../include/poker.h"
#include "benchmark.h"

/* Forward declarations of all benchmark functions */
BenchmarkResult benchmark_deck_shuffle(void);
BenchmarkResult benchmark_is_flush(void);
BenchmarkResult benchmark_is_straight(void);
BenchmarkResult benchmark_detect_royal_flush(void);
BenchmarkResult benchmark_detect_straight_flush(void);
BenchmarkResult benchmark_detect_four_of_a_kind(void);
BenchmarkResult benchmark_detect_full_house(void);
BenchmarkResult benchmark_detect_flush(void);
BenchmarkResult benchmark_detect_straight(void);
BenchmarkResult benchmark_detect_three_of_a_kind(void);
BenchmarkResult benchmark_detect_two_pair(void);
BenchmarkResult benchmark_detect_one_pair(void);
BenchmarkResult benchmark_detect_high_card(void);

int main(void) {
    BenchmarkResult results[13];
    size_t i = 0;

    printf("Running Poker Hand Evaluator Benchmarks...\n");
    printf("==========================================\n\n");

    printf("Each benchmark runs for minimum 1 second.\n");
    printf("Please wait...\n\n");

    /* Run deck operations */
    printf("[1/13] Benchmarking deck_shuffle...\n");
    results[i++] = benchmark_deck_shuffle();

    /* Run helper functions */
    printf("[2/13] Benchmarking is_flush...\n");
    results[i++] = benchmark_is_flush();

    printf("[3/13] Benchmarking is_straight...\n");
    results[i++] = benchmark_is_straight();

    /* Run detector functions (strongest to weakest) */
    printf("[4/13] Benchmarking detect_royal_flush...\n");
    results[i++] = benchmark_detect_royal_flush();

    printf("[5/13] Benchmarking detect_straight_flush...\n");
    results[i++] = benchmark_detect_straight_flush();

    printf("[6/13] Benchmarking detect_four_of_a_kind...\n");
    results[i++] = benchmark_detect_four_of_a_kind();

    printf("[7/13] Benchmarking detect_full_house...\n");
    results[i++] = benchmark_detect_full_house();

    printf("[8/13] Benchmarking detect_flush...\n");
    results[i++] = benchmark_detect_flush();

    printf("[9/13] Benchmarking detect_straight...\n");
    results[i++] = benchmark_detect_straight();

    printf("[10/13] Benchmarking detect_three_of_a_kind...\n");
    results[i++] = benchmark_detect_three_of_a_kind();

    printf("[11/13] Benchmarking detect_two_pair...\n");
    results[i++] = benchmark_detect_two_pair();

    printf("[12/13] Benchmarking detect_one_pair...\n");
    results[i++] = benchmark_detect_one_pair();

    printf("[13/13] Benchmarking detect_high_card...\n");
    results[i++] = benchmark_detect_high_card();

    /* Display results */
    print_benchmark_table(results, i);

    printf("Benchmark complete!\n");
    return 0;
}
