/*
 * deck.c - Deck management implementation
 * Provides functions for creating and managing poker decks
 */

#include "../include/poker.h"
#include <stdlib.h>

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
 * @param deck Pointer to deck to free
 */
void deck_free(Deck* deck) {
    if (deck == NULL) {
        return;
    }

    // Free cards array
    if (deck->cards != NULL) {
        free(deck->cards);
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
