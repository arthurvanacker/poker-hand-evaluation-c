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
 * HandCategory enumeration
 *
 * Represents poker hand categories from high card to royal flush.
 * Explicit numeric values enable direct comparison: higher value = stronger hand.
 * This allows simple numeric comparison (e.g., HAND_ROYAL_FLUSH > HAND_FLUSH).
 */
typedef enum {
    HAND_HIGH_CARD = 1,
    HAND_ONE_PAIR = 2,
    HAND_TWO_PAIR = 3,
    HAND_THREE_OF_A_KIND = 4,
    HAND_STRAIGHT = 5,
    HAND_FLUSH = 6,
    HAND_FULL_HOUSE = 7,
    HAND_FOUR_OF_A_KIND = 8,
    HAND_STRAIGHT_FLUSH = 9,
    HAND_ROYAL_FLUSH = 10
} HandCategory;


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
 * Maximum number of tiebreakers for hand comparison
 *
 * The maximum number of ranks needed to break ties between hands
 * of the same category. For example, high card hands may need up to
 * 5 tiebreakers (all 5 card ranks).
 */
#define MAX_TIEBREAKERS 5

/*
 * Hand structure
 *
 * Represents an evaluated poker hand with exactly 5 cards.
 * Contains the hand category (type) and tiebreaker ranks for comparison.
 *
 * Fields:
 * - cards: Fixed-size array containing exactly 5 cards
 * - category: The hand type (high card, pair, flush, etc.)
 * - tiebreakers: Ranks in descending importance order for breaking ties
 * - num_tiebreakers: Number of valid entries in tiebreakers array
 */
typedef struct {
    Card cards[5];              /* Fixed-size array (exactly 5 cards) */
    HandCategory category;       /* Hand type */
    Rank tiebreakers[MAX_TIEBREAKERS]; /* Ranks for tiebreaking */
    size_t num_tiebreakers;     /* Number of valid tiebreakers */
} Hand;


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
 *
 * Deallocates the deck's card array and the deck structure itself.
 * Safe to call with NULL pointer (no-op). After calling this function,
 * the deck pointer becomes invalid and must not be used.
 *
 * @param deck Pointer to deck to free (can be NULL)
 * @warning Deck pointer is invalid after calling this function
 */
void deck_free(Deck* deck);

/**
 * @brief Shuffle deck using Fisher-Yates algorithm
 *
 * Shuffles the deck in-place using the Fisher-Yates (Knuth) shuffle algorithm,
 * which provides a uniform random permutation of all 52 cards. The algorithm
 * iterates backwards through the array, swapping each element with a randomly
 * selected element from the unshuffled portion.
 *
 * Time complexity: O(n) where n is the number of cards
 * Space complexity: O(1) - shuffles in-place
 *
 * @param deck Pointer to deck to shuffle
 *
 * @note Caller must seed the random number generator with srand(time(NULL))
 *       before calling this function to ensure different shuffle results.
 *       This function uses rand() from <stdlib.h> for randomness.
 *
 * @warning Does not validate deck pointer. Caller must ensure deck is non-NULL.
 */
void deck_shuffle(Deck* deck);

/**
 * @brief Deal cards from deck
 * @param deck Deck to deal from
 * @param out_cards Output array for dealt cards (caller-allocated)
 * @param n Number of cards to deal
 * @return Actual number of cards dealt (may be less if not enough cards)
 */
size_t deck_deal(Deck* deck, Card* out_cards, size_t n);

#endif /* POKER_H */
