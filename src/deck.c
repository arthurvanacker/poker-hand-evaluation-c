/*
 * deck.c - Deck management implementation
 * Provides functions for creating and managing poker decks
 */

#include "../include/poker.h"
#include <stdlib.h>
#include <string.h>

/**
 * @brief Create new deck with 52 cards
 *
 * Allocates a new deck structure and populates it with all 52 standard
 * playing cards (13 ranks × 4 suits). Cards are generated in nested loop
 * order: outer loop iterates ranks (2-Ace), inner loop iterates suits.
 *
 * @return Pointer to new Deck, or NULL on allocation failure
 */
Deck* deck_new(void) {
    // Allocate deck structure
    Deck* deck = malloc(sizeof(Deck));
    if (deck == NULL) {
        return NULL;
    }

    // Allocate cards array for 52 cards
    deck->cards = malloc(52 * sizeof(Card));
    if (deck->cards == NULL) {
        free(deck);
        return NULL;
    }

    // Set size and capacity
    deck->size = 52;
    deck->capacity = 52;

    // Generate all 52 cards (4 suits × 13 ranks)
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
void deck_free(Deck* deck) {
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
void deck_shuffle(Deck* deck) {
    // Iterate backwards from last card to second card
    for (size_t i = deck->size - 1; i > 0; i--) {
        // Generate random index j where 0 <= j <= i
        size_t j = rand() % (i + 1);

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
size_t deck_deal(Deck* deck, Card* out_cards, size_t n) {
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
