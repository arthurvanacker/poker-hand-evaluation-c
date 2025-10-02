/*
 * Poker Hand Evaluation Library
 * Main header file with type definitions
 */

#ifndef POKER_H
#define POKER_H

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

#endif /* POKER_H */
