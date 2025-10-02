/*
 * Poker Hand Evaluation Library
 * Main header file with type definitions
 */

#ifndef POKER_H
#define POKER_H

#include <stddef.h>
#include <stdint.h>

/*
 * Rank enumeration
 *
 * Represents card ranks from 2 (deuce) to 14 (ace).
 * Explicit numeric values enable direct comparison and arithmetic operations.
 *
 * In standard poker, Ace can be high (14) or low (1) depending on context.
 * This implementation uses high ace (14) as the default representation.
 */
typedef enum {
    RANK_TWO = 2,
    RANK_THREE = 3,
    RANK_FOUR = 4,
    RANK_FIVE = 5,
    RANK_SIX = 6,
    RANK_SEVEN = 7,
    RANK_EIGHT = 8,
    RANK_NINE = 9,
    RANK_TEN = 10,
    RANK_JACK = 11,
    RANK_QUEEN = 12,
    RANK_KING = 13,
    RANK_ACE = 14
} Rank;

/*
 * Suit enumeration
 *
 * Represents the four standard suits in a poker deck.
 * Values are sequential starting from 0 for efficient array indexing.
 */
typedef enum {
    SUIT_HEARTS,
    SUIT_DIAMONDS,
    SUIT_CLUBS,
    SUIT_SPADES
} Suit;

/*
 * Card structure
 *
 * Represents a single playing card with rank and suit.
 * Designed to be compact (2 bytes) and passed by value efficiently.
 * Uses uint8_t to ensure minimal memory footprint.
 */
typedef struct {
    uint8_t rank;  /* Rank value (2-14) */
    uint8_t suit;  /* Suit value (0-3) */
} Card;

/**
 * @brief Convert card to string representation (e.g., "Ah", "Td")
 * @param card The card to convert
 * @param buffer Output buffer (must be at least 3 bytes)
 * @param size Size of output buffer
 * @return 0 on success, -1 on error
 */
int card_to_string(Card card, char* buffer, size_t size);

/**
 * @brief Parse string into Card struct
 * @param str Input string (e.g., "Ah", "Td")
 * @param out_card Pointer to Card to receive result
 * @return 0 on success, -1 on error
 */
int parse_card(const char* str, Card* out_card);

/*
 * Deck structure
 *
 * Represents a dynamic array of cards with capacity tracking.
 * Used for standard 52-card deck and can support deck manipulation operations.
 */
typedef struct {
    Card* cards;     /* Dynamic array of cards */
    size_t size;     /* Current number of cards */
    size_t capacity; /* Allocated capacity */
} Deck;

/**
 * @brief Create new deck with 52 cards
 * @return Pointer to new Deck, or NULL on allocation failure
 */
Deck* deck_new(void);

/**
 * @brief Free deck and all associated memory
 * @param deck Pointer to deck to free
 */
void deck_free(Deck* deck);

#endif /* POKER_H */
