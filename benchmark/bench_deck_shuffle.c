/*
 * Benchmark for deck_shuffle()
 * Measures shuffles per second using high-resolution timer
 */

#define _POSIX_C_SOURCE 199309L

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../include/poker.h"
#include "benchmark.h"

/*
 * Benchmark deck_shuffle performance
 * Runs shuffles for minimum 1 second and reports ops/sec
 */
BenchmarkResult benchmark_deck_shuffle(void) {
    Deck* deck;
    struct timespec start, end;
    int iterations = 0;
    int i;
    BenchmarkResult result;

    /* Initialize result */
    result.name = "deck_shuffle";
    result.ops_per_sec = 0.0;
    result.iterations = 0;
    result.elapsed_sec = 0.0;

    /* Create deck once */
    deck = deck_new();
    if (deck == NULL) {
        return result;
    }

    /* Seed RNG */
    srand((unsigned int)time(NULL));

    /* Benchmark: run for at least 1 second */
    clock_gettime(CLOCK_MONOTONIC, &start);
    do {
        for (i = 0; i < 100; i++) {
            deck_shuffle(deck);
            iterations++;
        }
        clock_gettime(CLOCK_MONOTONIC, &end);
    } while ((end.tv_sec - start.tv_sec) +
             (end.tv_nsec - start.tv_nsec) / 1e9 < 1.0);

    /* Calculate results */
    result.elapsed_sec = (end.tv_sec - start.tv_sec) +
                         (end.tv_nsec - start.tv_nsec) / 1e9;
    result.ops_per_sec = iterations / result.elapsed_sec;
    result.iterations = iterations;

    /* Cleanup */
    deck_free(deck);

    return result;
}
