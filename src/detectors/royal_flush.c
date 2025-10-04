/* royal_flush.c - Royal flush detector */

#include "../../include/poker.h"
#include <stddef.h>

/**
 * @brief Detect royal flush
 *
 * Detects if the hand is a royal flush (10-J-Q-K-A all same suit).
 * Uses is_flush() to verify all cards have the same suit, then checks
 * for the exact ranks TEN, JACK, QUEEN, KING, ACE.
 *
 * Royal flush is the strongest poker hand and requires no tiebreakers.
 *
 * @param cards Array of exactly HAND_SIZE cards
 * @param len Must be HAND_SIZE
 * @return 1 if royal flush, 0 otherwise
 */
int detect_royal_flush(const Card* const cards, const size_t len) {
    /* Validate input length */
    if (len != HAND_SIZE) {
        return 0;
    }

    /* Check if all cards are the same suit (flush) */
    if (!is_flush(cards, len)) {
        return 0;
    }

    /* Track which royal ranks we've found */
    int has_ten = 0;
    int has_jack = 0;
    int has_queen = 0;
    int has_king = 0;
    int has_ace = 0;

    /* Check each card for royal ranks */
    for (size_t i = 0; i < len; i++) {
        Rank rank = (Rank)cards[i].rank;
        if (rank == RANK_TEN) {
            has_ten = 1;
        } else if (rank == RANK_JACK) {
            has_jack = 1;
        } else if (rank == RANK_QUEEN) {
            has_queen = 1;
        } else if (rank == RANK_KING) {
            has_king = 1;
        } else if (rank == RANK_ACE) {
            has_ace = 1;
        } else {
            /* Non-royal rank found - cannot be royal flush */
            return 0;
        }
    }

    /* Verify all royal ranks are present */
    if (has_ten && has_jack && has_queen && has_king && has_ace) {
        return 1;
    }

    return 0;
}
