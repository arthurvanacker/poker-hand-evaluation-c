/* straight.c - Straight detector */

#include "../../include/poker.h"
#include <stddef.h>

/**
 * @brief Detect straight (non-flush)
 *
 * Detects if the hand is a straight but NOT a flush (excludes straight flushes).
 * Uses is_flush() to reject flushes, then is_straight() to detect sequential ranks.
 * Returns the high card as a tiebreaker (RANK_FIVE for wheel straight).
 *
 * @param cards Array of exactly HAND_SIZE cards
 * @param len Must be HAND_SIZE
 * @param out_tiebreakers Output array for tiebreaker ranks
 * @param out_num_tiebreakers Pointer to receive count of tiebreakers
 * @return 1 if straight (non-flush), 0 otherwise
 */
int detect_straight(const Card* const cards, const size_t len,
                    Rank* const out_tiebreakers,
                    size_t* const out_num_tiebreakers) {
    /* Validate input parameters */
    if (cards == NULL || len != HAND_SIZE || out_tiebreakers == NULL || out_num_tiebreakers == NULL) {
        return 0;
    }

    /* Check if it's a flush - if so, return 0 (exclude straight flushes) */
    if (is_flush(cards, len)) {
        return 0;
    }

    /* Check if cards form a straight */
    Rank high_card;
    if (!is_straight(cards, len, &high_card)) {
        return 0;
    }

    /* Write tiebreaker: [high_card] */
    out_tiebreakers[0] = high_card;
    *out_num_tiebreakers = 1;

    return 1;
}
