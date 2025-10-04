/*
 * deck.c - Deck management implementation
 * Provides functions for creating and managing poker decks
 */

#include "../include/poker.h"
#include <stdlib.h>
#include <string.h>

/**
 * @brief Generate unbiased random number in range [0, max) using rejection sampling
 *
 * Eliminates modulo bias that occurs with naive `rand() % max` approach.
 * When RAND_MAX+1 is not evenly divisible by max, some values appear more
 * frequently than others, introducing statistical bias in the shuffle.
 *
 * Example of modulo bias:
 * - RAND_MAX = 32767, max = 52
 * - 32767 % 52 = 47
 * - Values 0-47 appear ceil(32768/52) = 631 times
 * - Values 48-51 appear floor(32768/52) = 630 times
 * - Bias: ~0.16% difference
 *
 * Rejection sampling algorithm:
 * 1. Calculate limit = RAND_MAX - (RAND_MAX % max)
 * 2. Generate r = rand()
 * 3. If r >= limit, reject and retry (ensures uniform subset)
 * 4. Return r % max (now unbiased)
 *
 * The rejection probability is (RAND_MAX % max) / (RAND_MAX + 1), which is
 * very low for typical values (e.g., 47/32768 = 0.14% for 52-card deck).
 *
 * @param max Upper bound (exclusive) - returns value in [0, max)
 * @return Unbiased random number in range [0, max)
 */
size_t random_range(const size_t max) {
    // Calculate the largest multiple of max that fits in [0, RAND_MAX]
    // This is the rejection threshold - values >= limit are rejected
    size_t limit = RAND_MAX - (RAND_MAX % max);

    size_t r;
    do {
        r = (size_t)rand();
    } while (r >= limit);  // Rejection sampling: retry if r is in biased range

    return r % max;  // Now unbiased since r is uniformly distributed in [0, limit)
}

/**
 * @brief Create new deck with DECK_SIZE cards
 *
 * Allocates a new deck structure and populates it with all DECK_SIZE standard
 * playing cards (13 ranks × 4 suits). Cards are generated in nested loop
 * order: outer loop iterates ranks (2-Ace), inner loop iterates suits.
 *
 * @return Pointer to new Deck, or NULL on allocation failure
 */
Deck* deck_new(void) {
    // Allocate deck structure
    Deck* deck = malloc(sizeof(Deck));
    if (deck == NULL) {
        poker_errno = POKER_ENOMEM;
        return NULL;
    }

    // Allocate cards array for DECK_SIZE cards
    deck->cards = malloc(DECK_SIZE * sizeof(Card));
    if (deck->cards == NULL) {
        free(deck);
        poker_errno = POKER_ENOMEM;
        return NULL;
    }

    // Set size and capacity
    deck->size = DECK_SIZE;
    deck->capacity = DECK_SIZE;

    // Generate all DECK_SIZE cards (4 suits × 13 ranks)
    size_t index = 0;
    for (int rank = RANK_TWO; rank <= RANK_ACE; rank++) {
        for (int suit = SUIT_HEARTS; suit <= SUIT_SPADES; suit++) {
            deck->cards[index].rank = (uint8_t)rank;
            deck->cards[index].suit = (uint8_t)suit;
            index++;
        }
    }

    return deck;
}

/**
 * @brief Free deck and all associated memory
 *
 * Deallocates the cards array and the deck structure itself.
 * Safe to call with NULL pointer (no-op).
 *
 * Implements NULL poisoning: sets deck->cards to NULL after freeing
 * to prevent double-free vulnerabilities. Since free(NULL) is a no-op,
 * this makes accidental double-free calls safe for the cards array.
 *
 * @param deck Pointer to deck to free
 *
 * @note After calling this function, the deck pointer becomes invalid
 *       and must not be used. Caller should set deck = NULL to prevent
 *       use-after-free errors.
 */
void deck_free(Deck* const deck) {
    if (deck == NULL) {
        return;
    }

    // Free cards array and set to NULL (NULL poisoning)
    if (deck->cards != NULL) {
        free(deck->cards);
        deck->cards = NULL;  // Prevent double-free
    }

    // Free deck structure
    free(deck);
}

/**
 * @brief Shuffle deck using Fisher-Yates algorithm
 *
 * Implements the Fisher-Yates (Knuth) shuffle algorithm to randomly
 * permute the deck in-place. The algorithm works by iterating backwards
 * through the array, swapping each element with a randomly selected element
 * from positions 0 to i (inclusive).
 *
 * Algorithm:
 * for i from n-1 down to 1:
 *     j = random integer with 0 <= j <= i
 *     swap cards[i] with cards[j]
 *
 * This ensures each of the n! possible permutations has equal probability.
 *
 * @param deck Pointer to deck to shuffle (must be non-NULL)
 */
void deck_shuffle(Deck* const deck) {
    // Iterate backwards from last card to second card
    for (size_t i = deck->size - 1; i > 0; i--) {
        // Generate random index j where 0 <= j <= i using unbiased random_range()
        size_t j = random_range(i + 1);

        // Swap cards[i] with cards[j]
        Card temp = deck->cards[i];
        deck->cards[i] = deck->cards[j];
        deck->cards[j] = temp;
    }
}

/**
 * @brief Deal cards from deck
 *
 * Deals the requested number of cards from the deck by copying them to the
 * output array and reducing the deck size. Uses memcpy() for efficient bulk
 * copy of card data. If the deck contains fewer cards than requested, only
 * the available cards are dealt.
 *
 * The function deals cards from the beginning of the deck's card array
 * (index 0). After dealing, remaining cards are moved to the beginning of
 * the array using memmove(), and the deck's size field is reduced.
 *
 * @param deck Deck to deal from (must be non-NULL)
 * @param out_cards Output array for dealt cards (caller-allocated, must have space for n cards)
 * @param n Number of cards to deal
 * @return Actual number of cards dealt (may be less than n if deck has fewer cards)
 */
size_t deck_deal(Deck* const deck, Card* const out_cards, const size_t n) {
    // Determine actual number of cards to deal
    size_t actual_deal = (n < deck->size) ? n : deck->size;

    // Copy cards from deck to output array using memcpy
    if (actual_deal > 0) {
        memcpy(out_cards, deck->cards, actual_deal * sizeof(Card));

        // Move remaining cards to beginning of array
        size_t remaining = deck->size - actual_deal;
        if (remaining > 0) {
            memmove(deck->cards, deck->cards + actual_deal, remaining * sizeof(Card));
        }
    }

    // Reduce deck size by number of cards dealt
    deck->size -= actual_deal;

    return actual_deal;
}
