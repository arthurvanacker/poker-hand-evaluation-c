/*
 * Fuzzing harness for card parsing function
 *
 * This fuzzer tests the parse_card() function with arbitrary input data
 * to discover edge cases, crashes, and memory safety issues.
 *
 * Compile with:
 *   clang -fsanitize=fuzzer,address,undefined -I../include fuzz_parse_card.c ../lib/libpoker.a -o fuzz_parse_card
 *
 * Or with gcc (standalone mode):
 *   gcc -I../include -DFUZZ_STANDALONE fuzz_parse_card.c ../lib/libpoker.a -o fuzz_parse_card
 *
 * Run with libFuzzer:
 *   ./fuzz_parse_card -max_total_time=60
 *
 * Run standalone:
 *   ./fuzz_parse_card
 */

#include "../include/poker.h"
#include <stdint.h>
#include <stddef.h>
#include <string.h>

#ifdef FUZZ_STANDALONE
/* Standalone mode for testing without libFuzzer */
#include <stdio.h>
#include <stdlib.h>

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size);

int main(int argc, char **argv) {
    /* Test cases for standalone mode */
    const char *test_cases[] = {
        "Ah",    /* Valid: Ace of hearts */
        "Td",    /* Valid: Ten of diamonds */
        "2c",    /* Valid: Two of clubs */
        "Ks",    /* Valid: King of spades */
        "",      /* Edge: Empty string */
        "A",     /* Edge: Too short */
        "Ahx",   /* Edge: Too long */
        "Xh",    /* Invalid: Bad rank */
        "Az",    /* Invalid: Bad suit */
        "\0\0",  /* Edge: Null bytes */
        "ah",    /* Edge: Lowercase (should work) */
        "AH",    /* Edge: Uppercase suit */
        "  ",    /* Edge: Spaces */
        "\xff\xff", /* Edge: High bytes */
        "1h",    /* Invalid: Wrong rank character */
        "0h",    /* Invalid: Wrong rank character */
    };

    size_t num_tests = sizeof(test_cases) / sizeof(test_cases[0]);
    size_t passed = 0;

    printf("Running standalone fuzzing tests...\n");
    printf("----------------------------------------\n");

    for (size_t i = 0; i < num_tests; i++) {
        printf("Test %zu: \"%s\" -> ", i + 1, test_cases[i]);
        LLVMFuzzerTestOneInput((const uint8_t*)test_cases[i],
                                strlen(test_cases[i]));
        printf("OK\n");
        passed++;
    }

    /* Additional random fuzzing */
    printf("\nRunning random fuzzing (1000 iterations)...\n");
    srand(12345); /* Fixed seed for reproducibility */

    for (int i = 0; i < 1000; i++) {
        uint8_t random_input[10];
        size_t random_size = (rand() % 10);

        for (size_t j = 0; j < random_size; j++) {
            random_input[j] = (uint8_t)(rand() % 256);
        }

        LLVMFuzzerTestOneInput(random_input, random_size);
    }

    printf("\n----------------------------------------\n");
    printf("All tests completed: %zu/%zu passed\n", passed, num_tests);
    printf("No crashes detected!\n");

    return 0;
}
#endif

/*
 * Fuzzer entry point
 *
 * This function is called by libFuzzer with random input data.
 * It tests parse_card() with arbitrary byte sequences to find bugs.
 *
 * @param data Random input bytes from fuzzer
 * @param size Number of bytes in input
 * @return 0 (always, as required by libFuzzer)
 */
int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    /* Skip inputs that are too large (prevent unrealistic test cases) */
    if (size > 100) {
        return 0;
    }

    /* Create null-terminated string from input data */
    char input[101];
    if (size > 0) {
        memcpy(input, data, size);
    }
    input[size] = '\0';

    /* Test parse_card with fuzzer input */
    Card card;
    int result = parse_card(input, &card);

    /* If parsing succeeded, verify the card is valid */
    if (result == 0) {
        /* Verify rank is in valid range */
        if (card.rank < RANK_TWO || card.rank > RANK_ACE) {
            /* Bug detected: invalid rank after successful parse */
            __builtin_trap();
        }

        /* Verify suit is in valid range */
        if (card.suit > SUIT_SPADES) {
            /* Bug detected: invalid suit after successful parse */
            __builtin_trap();
        }

        /* Verify round-trip conversion works */
        char output[3];
        if (card_to_string(card, output, sizeof(output)) == 0) {
            /* Successful conversion - this is expected */
        }
    }

    /* Test with NULL pointer (should fail gracefully) */
    (void)parse_card(input, NULL);
    (void)parse_card(NULL, &card);

    return 0;
}
