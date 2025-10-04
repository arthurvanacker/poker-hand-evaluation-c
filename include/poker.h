/*
 * Poker Hand Evaluation Library
 * Main header file with type definitions
 */

#ifndef POKER_H
#define POKER_H

#include <stddef.h>
#include <stdint.h>

/*
 * Library version
 * Semantic versioning: MAJOR.MINOR.PATCH
 * - MAJOR: Incompatible API changes
 * - MINOR: Backwards-compatible functionality additions
 * - PATCH: Backwards-compatible bug fixes
 */
#define POKER_VERSION_MAJOR 0
#define POKER_VERSION_MINOR 3
#define POKER_VERSION_PATCH 0
#define POKER_VERSION "0.3.0"

/*
 * Version check macro for compile-time compatibility checks
 * Calculates version number: major*10000 + minor*100 + patch
 * Example: POKER_VERSION_CHECK(0, 3, 0) = 300
 */
#define POKER_VERSION_CHECK(major, minor, patch) \
    ((major) * 10000 + (minor) * 100 + (patch))

/*
 * Current version as integer for comparison
 * Use with POKER_VERSION_CHECK for conditional compilation:
 *   #if POKER_VERSION_NUMBER >= POKER_VERSION_CHECK(0, 3, 0)
 */
#define POKER_VERSION_NUMBER \
    POKER_VERSION_CHECK(POKER_VERSION_MAJOR, \
                        POKER_VERSION_MINOR, \
                        POKER_VERSION_PATCH)

/*
 * Constants
 */
#define RANK_ARRAY_SIZE 15  /* Array size for rank indexing (0-14, RANK_ACE=14) */
#define HAND_SIZE 5         /* Standard 5-card poker hand */
#define DECK_SIZE 52        /* Standard deck (4 suits × 13 ranks) */

/*
 * Error codes - Following errno conventions
 *
 * The poker library uses a global error indicator (poker_errno) similar
 * to the C standard library's errno. Functions that can fail will return
 * an error indicator (e.g., -1, NULL) and set poker_errno to indicate
 * the specific error type.
 */
extern int poker_errno;

#define POKER_EOK       0  /* No error */
#define POKER_EINVAL    1  /* Invalid argument */
#define POKER_ENOMEM    2  /* Out of memory */
#define POKER_ENOTFOUND 3  /* Pattern not found */
#define POKER_ERANGE    4  /* Out of range */

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
int card_to_string(const Card card, char* const buffer, const size_t size);

/**
 * @brief Parse string into Card struct
 * @param str Input string (e.g., "Ah", "Td")
 * @param out_card Pointer to Card to receive result
 * @return 0 on success, -1 on error
 */
int parse_card(const char* const str, Card* const out_card);

/*
 * Deck structure
 *
 * Represents a dynamic array of cards with capacity tracking.
 * Used for standard DECK_SIZE-card deck and can support deck manipulation operations.
 */
typedef struct {
    Card* cards;     /* Dynamic array of cards */
    size_t size;     /* Current number of cards */
    size_t capacity; /* Allocated capacity */
} Deck;

/**
 * @brief Create new deck with DECK_SIZE cards
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
 * This function implements NULL poisoning for the internal cards array
 * to prevent double-free vulnerabilities. However, the deck pointer itself
 * is freed and becomes invalid.
 *
 * @param deck Pointer to deck to free (can be NULL)
 *
 * @warning Deck pointer is invalid after calling this function. Caller
 *          must set deck = NULL to prevent use-after-free errors.
 *
 * @par Defensive Programming - Preventing Double-Free:
 * To safely use this function and prevent double-free errors, always set
 * the deck pointer to NULL after calling deck_free():
 * @code
 *     Deck* deck = deck_new();
 *     // ... use deck ...
 *     deck_free(deck);
 *     deck = NULL;  // REQUIRED: Prevents accidental double-free
 * @endcode
 */
void deck_free(Deck* const deck);

/**
 * @brief Generate unbiased random number in range [0, max)
 *
 * Uses rejection sampling to eliminate modulo bias that occurs with
 * naive `rand() % max` approach. The modulo operation introduces bias
 * when RAND_MAX+1 is not evenly divisible by max, causing some values
 * to be slightly more likely than others.
 *
 * Algorithm:
 * 1. Calculate limit = RAND_MAX - (RAND_MAX % max)
 * 2. Generate random number r = rand()
 * 3. If r >= limit, reject and retry (ensures uniform distribution)
 * 4. Return r % max (now unbiased since r is from uniform subset)
 *
 * Example of modulo bias:
 * - If RAND_MAX = 32767 and max = 10:
 *   - RAND_MAX % 10 = 7
 *   - Values 0-7 appear 3278 times each
 *   - Values 8-9 appear 3277 times each
 *   - Bias: ~0.03% difference
 *
 * For standard 52-card deck:
 * - RAND_MAX = 32767, max = 52
 * - RAND_MAX % 52 = 47
 * - Bias is negligible but non-zero (~0.15% difference)
 *
 * Rejection sampling eliminates this bias entirely by only using
 * random values from [0, limit) where limit is the largest multiple
 * of max that is <= RAND_MAX.
 *
 * Time complexity: O(1) expected (rejection probability is very low)
 * Space complexity: O(1)
 *
 * @param max Upper bound (exclusive) - returns value in [0, max)
 * @return Unbiased random number in range [0, max)
 *
 * @note This is a static helper function used internally by deck_shuffle.
 *       For testing purposes, it's exposed in the header.
 */
size_t random_range(const size_t max);

/**
 * @brief Shuffle deck using Fisher-Yates algorithm
 *
 * Shuffles the deck in-place using the Fisher-Yates (Knuth) shuffle algorithm,
 * which provides a uniform random permutation of all DECK_SIZE cards. The algorithm
 * iterates backwards through the array, swapping each element with a randomly
 * selected element from the unshuffled portion.
 *
 * Uses unbiased random_range() function to eliminate modulo bias that
 * occurred in previous implementation using `rand() % (i+1)`.
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
void deck_shuffle(Deck* const deck);

/**
 * @brief Deal cards from deck
 * @param deck Deck to deal from
 * @param out_cards Output array for dealt cards (caller-allocated)
 * @param n Number of cards to deal
 * @return Actual number of cards dealt (may be less if not enough cards)
 */
size_t deck_deal(Deck* const deck, Card* const out_cards, const size_t n);

/**
 * @brief Check if all cards are the same suit
 * @param cards Array of cards
 * @param len Number of cards (must be HAND_SIZE)
 * @return 1 if flush, 0 otherwise
 */
int is_flush(const Card* const cards, const size_t len);

/**
 * @brief Check if cards form a straight
 * @param cards Array of cards to check
 * @param len Number of cards (must be HAND_SIZE)
 * @param out_high_card Optional pointer to receive high card rank
 * @return 1 if straight detected, 0 otherwise
 */
int is_straight(const Card* const cards, const size_t len, Rank* const out_high_card);

/**
 * @brief Count frequency of each rank
 * @param cards Array of cards
 * @param len Number of cards
 * @param counts Output array[RANK_ARRAY_SIZE] to receive counts (indexed by Rank)
 */
void rank_counts(const Card* const cards, const size_t len, int* const counts);

/**
 * @brief Detect four of a kind
 * @param cards Array of exactly HAND_SIZE cards
 * @param len Must be HAND_SIZE
 * @param counts Optional pre-computed rank counts (can be NULL)
 * @param out_tiebreakers Output array for tiebreaker ranks
 * @param out_num_tiebreakers Pointer to receive count of tiebreakers
 * @return 1 if four of a kind, 0 otherwise
 */
int detect_four_of_a_kind(const Card* const cards, const size_t len,
                           const int* const counts,
                           Rank* const out_tiebreakers,
                           size_t* const out_num_tiebreakers);

/**
 * @brief Detect full house
 * @param cards Array of exactly HAND_SIZE cards
 * @param len Must be HAND_SIZE
 * @param counts Optional pre-computed rank counts (can be NULL)
 * @param out_tiebreakers Output array for tiebreaker ranks
 * @param out_num_tiebreakers Pointer to receive count of tiebreakers
 * @return 1 if full house, 0 otherwise
 */
int detect_full_house(const Card* const cards, const size_t len,
                      const int* const counts,
                      Rank* const out_tiebreakers,
                      size_t* const out_num_tiebreakers);

/**
 * @brief Detect three of a kind (no pair)
 * @param cards Array of exactly HAND_SIZE cards
 * @param len Must be HAND_SIZE
 * @param counts Optional pre-computed rank counts (can be NULL)
 * @param out_tiebreakers Output array for tiebreaker ranks
 * @param out_num_tiebreakers Pointer to receive count of tiebreakers
 * @return 1 if three of a kind, 0 otherwise
 */
int detect_three_of_a_kind(const Card* const cards, const size_t len,
                            const int* const counts,
                            Rank* const out_tiebreakers,
                            size_t* const out_num_tiebreakers);

/**
 * @brief Detect two pair
 * @param cards Array of exactly HAND_SIZE cards
 * @param len Must be HAND_SIZE
 * @param counts Optional pre-computed rank counts (can be NULL)
 * @param out_tiebreakers Output array for tiebreaker ranks
 * @param out_num_tiebreakers Pointer to receive count of tiebreakers
 * @return 1 if two pair, 0 otherwise
 */
int detect_two_pair(const Card* const cards, const size_t len,
                    const int* const counts,
                    Rank* const out_tiebreakers,
                    size_t* const out_num_tiebreakers);

/**
 * @brief Detect one pair
 * @param cards Array of exactly HAND_SIZE cards
 * @param len Must be HAND_SIZE
 * @param counts Optional pre-computed rank counts (can be NULL)
 * @param out_tiebreakers Output array for tiebreaker ranks
 * @param out_num_tiebreakers Pointer to receive count of tiebreakers
 * @return 1 if one pair, 0 otherwise
 */
int detect_one_pair(const Card* const cards, const size_t len,
                    const int* const counts,
                    Rank* const out_tiebreakers,
                    size_t* const out_num_tiebreakers);

/**
 * @brief Detect straight flush
 * @param cards Array of exactly HAND_SIZE cards
 * @param len Must be HAND_SIZE
 * @param out_high_card Pointer to receive high card rank (can be NULL)
 * @return 1 if straight flush, 0 otherwise
 */
int detect_straight_flush(const Card* const cards, const size_t len, Rank* const out_high_card);

/**
 * @brief Detect royal flush
 * @param cards Array of exactly HAND_SIZE cards
 * @param len Must be HAND_SIZE
 * @return 1 if royal flush, 0 otherwise
 */
int detect_royal_flush(const Card* const cards, const size_t len);

/**
 * @brief Detect flush (non-straight)
 * @param cards Array of exactly HAND_SIZE cards
 * @param len Must be HAND_SIZE
 * @param out_tiebreakers Output array for tiebreaker ranks
 * @param out_num_tiebreakers Pointer to receive count of tiebreakers
 * @return 1 if flush, 0 otherwise
 */
int detect_flush(const Card* const cards, const size_t len,
                 Rank* const out_tiebreakers,
                 size_t* const out_num_tiebreakers);

/**
 * @brief Detect straight (non-flush)
 * @param cards Array of exactly HAND_SIZE cards
 * @param len Must be HAND_SIZE
 * @param out_tiebreakers Output array for tiebreaker ranks
 * @param out_num_tiebreakers Pointer to receive count of tiebreakers
 * @return 1 if straight, 0 otherwise
 */
int detect_straight(const Card* const cards, const size_t len,
                    Rank* const out_tiebreakers,
                    size_t* const out_num_tiebreakers);

/**
 * @brief Detect high card (always succeeds for valid input)
 * @param cards Array of exactly HAND_SIZE cards
 * @param len Must be HAND_SIZE
 * @param out_tiebreakers Output array for tiebreaker ranks
 * @param out_num_tiebreakers Pointer to receive count of tiebreakers
 * @return 1 if valid HAND_SIZE-card input, 0 otherwise
 */
int detect_high_card(const Card* const cards, const size_t len,
                     Rank* const out_tiebreakers,
                     size_t* const out_num_tiebreakers);

/*
 * Maximum number of tiebreaker ranks in Hand struct
 */
#define MAX_TIEBREAKERS HAND_SIZE

/*
 * Hand structure
 *
 * Represents an evaluated HAND_SIZE-card poker hand with its category and tiebreakers.
 *
 * Fields:
 * - cards: Fixed-size array containing exactly HAND_SIZE cards that form the hand
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
    Card cards[HAND_SIZE];              /* Exactly HAND_SIZE cards */
    HandCategory category;               /* Hand type */
    Rank tiebreakers[MAX_TIEBREAKERS];  /* Ranks for tiebreaking */
    size_t num_tiebreakers;             /* Number of valid tiebreakers */
} Hand;

#endif /* POKER_H */
