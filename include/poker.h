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
 * Represents the ranking categories of poker hands.
 * Explicit numeric values (1-10) enable direct comparison where higher values
 * represent stronger hands. This allows simple numeric comparison to determine
 * which hand wins (e.g., HAND_ROYAL_FLUSH > HAND_FLUSH).
 *
 * Hands are ranked from weakest (1) to strongest (10) according to standard
 * poker rules.
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

/**
 * @brief Check if all cards are the same suit
 * @param cards Array of cards
 * @param len Number of cards (must be 5)
 * @return 1 if flush, 0 otherwise
 */
int is_flush(const Card* cards, size_t len);

/**
 * @brief Check if cards form a straight
 * @param cards Array of cards to check
 * @param len Number of cards (must be 5)
 * @param out_high_card Optional pointer to receive high card rank
 * @return 1 if straight detected, 0 otherwise
 */
int is_straight(const Card* cards, size_t len, Rank* out_high_card);

/**
 * @brief Count frequency of each rank
 * @param cards Array of cards
 * @param len Number of cards
 * @param counts Output array[15] to receive counts (indexed by Rank)
 */
void rank_counts(const Card* cards, size_t len, int* counts);

/**
 * @brief Detect four of a kind
 * @param cards Array of exactly 5 cards
 * @param len Must be 5
 * @param counts Optional pre-computed rank counts (can be NULL)
 * @param out_tiebreakers Output array for tiebreaker ranks
 * @param out_num_tiebreakers Pointer to receive count of tiebreakers
 * @return 1 if four of a kind, 0 otherwise
 */
int detect_four_of_a_kind(const Card* cards, size_t len,
                           const int* counts,
                           Rank* out_tiebreakers,
                           size_t* out_num_tiebreakers);

/**
 * @brief Detect straight flush
 * @param cards Array of exactly 5 cards
 * @param len Must be 5
 * @param out_high_card Pointer to receive high card rank (can be NULL)
 * @return 1 if straight flush, 0 otherwise
 */
int detect_straight_flush(const Card* cards, size_t len, Rank* out_high_card);

/**
 * @brief Detect royal flush
 * @param cards Array of exactly 5 cards
 * @param len Must be 5
 * @return 1 if royal flush, 0 otherwise
 */
int detect_royal_flush(const Card* cards, size_t len);

/*
 * Maximum number of tiebreaker ranks in Hand struct
 */
#define MAX_TIEBREAKERS 5

/*
 * Hand structure
 *
 * Represents an evaluated 5-card poker hand with its category and tiebreakers.
 *
 * Fields:
 * - cards: Fixed-size array containing exactly 5 cards that form the hand
 * - category: The hand type (HAND_HIGH_CARD to HAND_ROYAL_FLUSH) that identifies
 *             the poker hand ranking category
 * - tiebreakers: Array of ranks in descending order of importance used to break
 *                ties between hands of the same category. For example, in a pair
 *                of Kings with A-Q-J kickers, tiebreakers would be [K, A, Q, J]
 * - num_tiebreakers: Number of valid tiebreaker ranks in the tiebreakers array
 *                    (0 to MAX_TIEBREAKERS)
 *
 * Design notes:
 * - Uses fixed-size arrays to avoid dynamic memory allocation
 * - Tiebreaker count is tracked explicitly for clarity and bounds checking
 * - Ranks in tiebreakers are ordered by importance for efficient comparison
 */
typedef struct {
    Card cards[5];                      /* Exactly 5 cards */
    HandCategory category;               /* Hand type */
    Rank tiebreakers[MAX_TIEBREAKERS];  /* Ranks for tiebreaking */
    size_t num_tiebreakers;             /* Number of valid tiebreakers */
} Hand;

#endif /* POKER_H */
