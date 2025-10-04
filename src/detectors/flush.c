/* flush.c - Flush detector */

#include "../../include/poker.h"
#include <stdlib.h>
#include <stddef.h>

/* External helper for rank comparison */
extern int rank_compare_desc(const void* a, const void* b);

/**
 * @brief Detect flush (non-straight)
 *
 * Detects if the hand is a flush (HAND_SIZE suited cards, non-sequential).
 * Uses is_flush() to verify all cards have the same suit, then uses
 * is_straight() to exclude straight flushes. Returns all HAND_SIZE ranks in
 * descending order as tiebreakers.
 *
 * @param cards Array of exactly HAND_SIZE cards
 * @param len Must be HAND_SIZE
 * @param out_tiebreakers Output array for tiebreaker ranks
 * @param out_num_tiebreakers Pointer to receive count of tiebreakers
 * @return 1 if flush, 0 otherwise
 */
int detect_flush(const Card* const cards, const size_t len,
                 Rank* const out_tiebreakers,
                 size_t* const out_num_tiebreakers) {
    /* Validate input parameters */
    if (cards == NULL || len != HAND_SIZE || out_tiebreakers == NULL || out_num_tiebreakers == NULL) {
        return 0;
    }

    /* Check if all cards are the same suit (flush) */
    if (!is_flush(cards, len)) {
        return 0;
    }

    /* Exclude straight flushes (return 0 if straight) */
    if (is_straight(cards, len, NULL)) {
        return 0;
    }

    /* Extract ranks into array */
    Rank ranks[HAND_SIZE];
    for (size_t i = 0; i < HAND_SIZE; i++) {
        ranks[i] = (Rank)cards[i].rank;
    }

    /* Sort ranks in descending order */
    qsort(ranks, HAND_SIZE, sizeof(Rank), rank_compare_desc);

    /* Write all HAND_SIZE ranks to tiebreakers */
    for (size_t i = 0; i < HAND_SIZE; i++) {
        out_tiebreakers[i] = ranks[i];
    }

    /* Set number of tiebreakers to 5 */
    *out_num_tiebreakers = HAND_SIZE;

    return 1;
}
