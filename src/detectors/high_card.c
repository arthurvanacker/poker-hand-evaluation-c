/* high_card.c - High card detector */

#include "../../include/poker.h"
#include <stdlib.h>
#include <stddef.h>

/* External helper for rank comparison */
extern int rank_compare_desc(const void* a, const void* b);

/**
 * @brief Detect high card (always succeeds for valid input)
 *
 * Detects high card, which is the fallback when no other hand category matches.
 * Returns all HAND_SIZE card ranks in descending order as tiebreakers. This function
 * always succeeds for valid HAND_SIZE-card input, making it the default/weakest hand.
 *
 * @param cards Array of exactly HAND_SIZE cards
 * @param len Must be HAND_SIZE
 * @param out_tiebreakers Output array for tiebreaker ranks
 * @param out_num_tiebreakers Pointer to receive count of tiebreakers
 * @return 1 if valid HAND_SIZE-card input, 0 otherwise
 */
int detect_high_card(const Card* const cards, const size_t len,
                     Rank* const out_tiebreakers,
                     size_t* const out_num_tiebreakers) {
    /* Validate input parameters */
    if (cards == NULL || len != HAND_SIZE || out_tiebreakers == NULL || out_num_tiebreakers == NULL) {
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
