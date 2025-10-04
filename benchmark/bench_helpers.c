/*
 * Benchmarks for helper functions (is_flush, is_straight)
 * Measures checks per second using high-resolution timer
 */

#define _POSIX_C_SOURCE 199309L

#include <stdio.h>
#include <time.h>
#include "../include/poker.h"
#include "benchmark.h"

/*
 * Benchmark is_flush performance
 */
BenchmarkResult benchmark_is_flush(void) {
    struct timespec start, end;
    int iterations = 0;
    int i;
    BenchmarkResult result;

    /* Test data: flush hand */
    Card cards[HAND_SIZE] = {
        {RANK_ACE, SUIT_HEARTS},
        {RANK_KING, SUIT_HEARTS},
        {RANK_QUEEN, SUIT_HEARTS},
        {RANK_JACK, SUIT_HEARTS},
        {RANK_NINE, SUIT_HEARTS}
    };

    result.name = "is_flush";

    /* Benchmark: run for at least 1 second */
    clock_gettime(CLOCK_MONOTONIC, &start);
    do {
        for (i = 0; i < 100000; i++) {
            is_flush(cards, HAND_SIZE);
            iterations++;
        }
        clock_gettime(CLOCK_MONOTONIC, &end);
    } while ((end.tv_sec - start.tv_sec) +
             (end.tv_nsec - start.tv_nsec) / 1e9 < 1.0);

    result.elapsed_sec = (end.tv_sec - start.tv_sec) +
                         (end.tv_nsec - start.tv_nsec) / 1e9;
    result.ops_per_sec = iterations / result.elapsed_sec;
    result.iterations = iterations;

    return result;
}

/*
 * Benchmark is_straight performance
 */
BenchmarkResult benchmark_is_straight(void) {
    struct timespec start, end;
    int iterations = 0;
    int i;
    Rank high_card;
    BenchmarkResult result;

    /* Test data: straight hand */
    Card cards[HAND_SIZE] = {
        {RANK_TEN, SUIT_HEARTS},
        {RANK_JACK, SUIT_DIAMONDS},
        {RANK_QUEEN, SUIT_CLUBS},
        {RANK_KING, SUIT_SPADES},
        {RANK_ACE, SUIT_HEARTS}
    };

    result.name = "is_straight";

    /* Benchmark: run for at least 1 second */
    clock_gettime(CLOCK_MONOTONIC, &start);
    do {
        for (i = 0; i < 100000; i++) {
            is_straight(cards, HAND_SIZE, &high_card);
            iterations++;
        }
        clock_gettime(CLOCK_MONOTONIC, &end);
    } while ((end.tv_sec - start.tv_sec) +
             (end.tv_nsec - start.tv_nsec) / 1e9 < 1.0);

    result.elapsed_sec = (end.tv_sec - start.tv_sec) +
                         (end.tv_nsec - start.tv_nsec) / 1e9;
    result.ops_per_sec = iterations / result.elapsed_sec;
    result.iterations = iterations;

    return result;
}
