/*
 * Benchmarks for all 10 detect_* functions
 * Measures detections per second using high-resolution timer
 */

#define _POSIX_C_SOURCE 199309L

#include <stdio.h>
#include <time.h>
#include "../include/poker.h"
#include "benchmark.h"

/* Helper macro to run benchmark loop */
#define RUN_BENCHMARK(func_call) \
    do { \
        clock_gettime(CLOCK_MONOTONIC, &start); \
        do { \
            for (i = 0; i < 100000; i++) { \
                func_call; \
                iterations++; \
            } \
            clock_gettime(CLOCK_MONOTONIC, &end); \
        } while ((end.tv_sec - start.tv_sec) + \
                 (end.tv_nsec - start.tv_nsec) / 1e9 < 1.0); \
    } while (0)

/*
 * Benchmark detect_royal_flush
 */
BenchmarkResult benchmark_detect_royal_flush(void) {
    struct timespec start, end;
    int iterations = 0;
    int i;
    BenchmarkResult result;

    /* Royal flush hand */
    Card cards[HAND_SIZE] = {
        {RANK_TEN, SUIT_HEARTS},
        {RANK_JACK, SUIT_HEARTS},
        {RANK_QUEEN, SUIT_HEARTS},
        {RANK_KING, SUIT_HEARTS},
        {RANK_ACE, SUIT_HEARTS}
    };

    result.name = "detect_royal_flush";

    RUN_BENCHMARK(detect_royal_flush(cards, HAND_SIZE));

    result.elapsed_sec = (end.tv_sec - start.tv_sec) +
                         (end.tv_nsec - start.tv_nsec) / 1e9;
    result.ops_per_sec = iterations / result.elapsed_sec;
    result.iterations = iterations;

    return result;
}

/*
 * Benchmark detect_straight_flush
 */
BenchmarkResult benchmark_detect_straight_flush(void) {
    struct timespec start, end;
    int iterations = 0;
    int i;
    Rank high_card;
    BenchmarkResult result;

    /* Straight flush hand */
    Card cards[HAND_SIZE] = {
        {RANK_NINE, SUIT_HEARTS},
        {RANK_EIGHT, SUIT_HEARTS},
        {RANK_SEVEN, SUIT_HEARTS},
        {RANK_SIX, SUIT_HEARTS},
        {RANK_FIVE, SUIT_HEARTS}
    };

    result.name = "detect_straight_flush";

    RUN_BENCHMARK(detect_straight_flush(cards, HAND_SIZE, &high_card));

    result.elapsed_sec = (end.tv_sec - start.tv_sec) +
                         (end.tv_nsec - start.tv_nsec) / 1e9;
    result.ops_per_sec = iterations / result.elapsed_sec;
    result.iterations = iterations;

    return result;
}

/*
 * Benchmark detect_four_of_a_kind
 */
BenchmarkResult benchmark_detect_four_of_a_kind(void) {
    struct timespec start, end;
    int iterations = 0;
    int i;
    Rank tiebreakers[MAX_TIEBREAKERS];
    size_t num_tiebreakers;
    BenchmarkResult result;

    /* Four of a kind hand */
    Card cards[HAND_SIZE] = {
        {RANK_ACE, SUIT_HEARTS},
        {RANK_ACE, SUIT_DIAMONDS},
        {RANK_ACE, SUIT_CLUBS},
        {RANK_ACE, SUIT_SPADES},
        {RANK_KING, SUIT_HEARTS}
    };

    result.name = "detect_four_of_a_kind";

    RUN_BENCHMARK(detect_four_of_a_kind(cards, HAND_SIZE, NULL, tiebreakers, &num_tiebreakers));

    result.elapsed_sec = (end.tv_sec - start.tv_sec) +
                         (end.tv_nsec - start.tv_nsec) / 1e9;
    result.ops_per_sec = iterations / result.elapsed_sec;
    result.iterations = iterations;

    return result;
}

/*
 * Benchmark detect_full_house
 */
BenchmarkResult benchmark_detect_full_house(void) {
    struct timespec start, end;
    int iterations = 0;
    int i;
    Rank tiebreakers[MAX_TIEBREAKERS];
    size_t num_tiebreakers;
    BenchmarkResult result;

    /* Full house hand */
    Card cards[HAND_SIZE] = {
        {RANK_KING, SUIT_HEARTS},
        {RANK_KING, SUIT_DIAMONDS},
        {RANK_KING, SUIT_CLUBS},
        {RANK_QUEEN, SUIT_SPADES},
        {RANK_QUEEN, SUIT_HEARTS}
    };

    result.name = "detect_full_house";

    RUN_BENCHMARK(detect_full_house(cards, HAND_SIZE, NULL, tiebreakers, &num_tiebreakers));

    result.elapsed_sec = (end.tv_sec - start.tv_sec) +
                         (end.tv_nsec - start.tv_nsec) / 1e9;
    result.ops_per_sec = iterations / result.elapsed_sec;
    result.iterations = iterations;

    return result;
}

/*
 * Benchmark detect_flush
 */
BenchmarkResult benchmark_detect_flush(void) {
    struct timespec start, end;
    int iterations = 0;
    int i;
    Rank tiebreakers[MAX_TIEBREAKERS];
    size_t num_tiebreakers;
    BenchmarkResult result;

    /* Flush hand (non-straight) */
    Card cards[HAND_SIZE] = {
        {RANK_ACE, SUIT_HEARTS},
        {RANK_KING, SUIT_HEARTS},
        {RANK_QUEEN, SUIT_HEARTS},
        {RANK_JACK, SUIT_HEARTS},
        {RANK_NINE, SUIT_HEARTS}
    };

    result.name = "detect_flush";

    RUN_BENCHMARK(detect_flush(cards, HAND_SIZE, tiebreakers, &num_tiebreakers));

    result.elapsed_sec = (end.tv_sec - start.tv_sec) +
                         (end.tv_nsec - start.tv_nsec) / 1e9;
    result.ops_per_sec = iterations / result.elapsed_sec;
    result.iterations = iterations;

    return result;
}

/*
 * Benchmark detect_straight
 */
BenchmarkResult benchmark_detect_straight(void) {
    struct timespec start, end;
    int iterations = 0;
    int i;
    Rank tiebreakers[MAX_TIEBREAKERS];
    size_t num_tiebreakers;
    BenchmarkResult result;

    /* Straight hand (non-flush) */
    Card cards[HAND_SIZE] = {
        {RANK_TEN, SUIT_HEARTS},
        {RANK_JACK, SUIT_DIAMONDS},
        {RANK_QUEEN, SUIT_CLUBS},
        {RANK_KING, SUIT_SPADES},
        {RANK_ACE, SUIT_HEARTS}
    };

    result.name = "detect_straight";

    RUN_BENCHMARK(detect_straight(cards, HAND_SIZE, tiebreakers, &num_tiebreakers));

    result.elapsed_sec = (end.tv_sec - start.tv_sec) +
                         (end.tv_nsec - start.tv_nsec) / 1e9;
    result.ops_per_sec = iterations / result.elapsed_sec;
    result.iterations = iterations;

    return result;
}

/*
 * Benchmark detect_three_of_a_kind
 */
BenchmarkResult benchmark_detect_three_of_a_kind(void) {
    struct timespec start, end;
    int iterations = 0;
    int i;
    Rank tiebreakers[MAX_TIEBREAKERS];
    size_t num_tiebreakers;
    BenchmarkResult result;

    /* Three of a kind hand */
    Card cards[HAND_SIZE] = {
        {RANK_KING, SUIT_HEARTS},
        {RANK_KING, SUIT_DIAMONDS},
        {RANK_KING, SUIT_CLUBS},
        {RANK_ACE, SUIT_SPADES},
        {RANK_QUEEN, SUIT_HEARTS}
    };

    result.name = "detect_three_of_a_kind";

    RUN_BENCHMARK(detect_three_of_a_kind(cards, HAND_SIZE, NULL, tiebreakers, &num_tiebreakers));

    result.elapsed_sec = (end.tv_sec - start.tv_sec) +
                         (end.tv_nsec - start.tv_nsec) / 1e9;
    result.ops_per_sec = iterations / result.elapsed_sec;
    result.iterations = iterations;

    return result;
}

/*
 * Benchmark detect_two_pair
 */
BenchmarkResult benchmark_detect_two_pair(void) {
    struct timespec start, end;
    int iterations = 0;
    int i;
    Rank tiebreakers[MAX_TIEBREAKERS];
    size_t num_tiebreakers;
    BenchmarkResult result;

    /* Two pair hand */
    Card cards[HAND_SIZE] = {
        {RANK_ACE, SUIT_HEARTS},
        {RANK_ACE, SUIT_DIAMONDS},
        {RANK_KING, SUIT_CLUBS},
        {RANK_KING, SUIT_SPADES},
        {RANK_QUEEN, SUIT_HEARTS}
    };

    result.name = "detect_two_pair";

    RUN_BENCHMARK(detect_two_pair(cards, HAND_SIZE, NULL, tiebreakers, &num_tiebreakers));

    result.elapsed_sec = (end.tv_sec - start.tv_sec) +
                         (end.tv_nsec - start.tv_nsec) / 1e9;
    result.ops_per_sec = iterations / result.elapsed_sec;
    result.iterations = iterations;

    return result;
}

/*
 * Benchmark detect_one_pair
 */
BenchmarkResult benchmark_detect_one_pair(void) {
    struct timespec start, end;
    int iterations = 0;
    int i;
    Rank tiebreakers[MAX_TIEBREAKERS];
    size_t num_tiebreakers;
    BenchmarkResult result;

    /* One pair hand */
    Card cards[HAND_SIZE] = {
        {RANK_ACE, SUIT_HEARTS},
        {RANK_ACE, SUIT_DIAMONDS},
        {RANK_KING, SUIT_CLUBS},
        {RANK_QUEEN, SUIT_SPADES},
        {RANK_JACK, SUIT_HEARTS}
    };

    result.name = "detect_one_pair";

    RUN_BENCHMARK(detect_one_pair(cards, HAND_SIZE, NULL, tiebreakers, &num_tiebreakers));

    result.elapsed_sec = (end.tv_sec - start.tv_sec) +
                         (end.tv_nsec - start.tv_nsec) / 1e9;
    result.ops_per_sec = iterations / result.elapsed_sec;
    result.iterations = iterations;

    return result;
}

/*
 * Benchmark detect_high_card
 */
BenchmarkResult benchmark_detect_high_card(void) {
    struct timespec start, end;
    int iterations = 0;
    int i;
    Rank tiebreakers[MAX_TIEBREAKERS];
    size_t num_tiebreakers;
    BenchmarkResult result;

    /* High card hand */
    Card cards[HAND_SIZE] = {
        {RANK_ACE, SUIT_HEARTS},
        {RANK_KING, SUIT_DIAMONDS},
        {RANK_QUEEN, SUIT_CLUBS},
        {RANK_JACK, SUIT_SPADES},
        {RANK_NINE, SUIT_HEARTS}
    };

    result.name = "detect_high_card";

    RUN_BENCHMARK(detect_high_card(cards, HAND_SIZE, tiebreakers, &num_tiebreakers));

    result.elapsed_sec = (end.tv_sec - start.tv_sec) +
                         (end.tv_nsec - start.tv_nsec) / 1e9;
    result.ops_per_sec = iterations / result.elapsed_sec;
    result.iterations = iterations;

    return result;
}
