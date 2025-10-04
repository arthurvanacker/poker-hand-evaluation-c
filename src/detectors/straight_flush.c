/* straight_flush.c - Straight flush detector */

#include "../../include/poker.h"
#include <stddef.h>

/**
 * @brief Detect straight flush
 *
 * Detects if the hand is a straight flush (HAND_SIZE sequential suited cards).
 * Combines is_flush() and is_straight() checks. Returns the high card
 * for tiebreakers (RANK_FIVE for wheel straight flush).
 *
 * @param cards Array of exactly HAND_SIZE cards
 * @param len Must be HAND_SIZE
 * @param out_high_card Pointer to receive high card rank (can be NULL)
 * @return 1 if straight flush, 0 otherwise
 */
int detect_straight_flush(const Card* const cards, const size_t len, Rank* const out_high_card) {
    /* Validate input length */
    if (len != HAND_SIZE) {
        return 0;
    }

    /* Check if all cards are the same suit (flush) */
    if (!is_flush(cards, len)) {
        return 0;
    }

    /* Check if cards form a straight */
    if (!is_straight(cards, len, out_high_card)) {
        return 0;
    }

    /* Both flush and straight confirmed - it's a straight flush */
    return 1;
}
