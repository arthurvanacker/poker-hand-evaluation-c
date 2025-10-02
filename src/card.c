/* card.c - Card representation and utilities */

#include <stdio.h>
#include "../include/poker.h"

void card_init(void) {
    /* Placeholder for card initialization */
}

int card_to_string(Card card, char* buffer, size_t size) {
    // Check buffer size - need at least 3 bytes (2 chars + null terminator)
    if (size < 3) {
        return -1;
    }

    // Map rank to character
    char rank_char;
    switch (card.rank) {
        case RANK_TWO:   rank_char = '2'; break;
        case RANK_THREE: rank_char = '3'; break;
        case RANK_FOUR:  rank_char = '4'; break;
        case RANK_FIVE:  rank_char = '5'; break;
        case RANK_SIX:   rank_char = '6'; break;
        case RANK_SEVEN: rank_char = '7'; break;
        case RANK_EIGHT: rank_char = '8'; break;
        case RANK_NINE:  rank_char = '9'; break;
        case RANK_TEN:   rank_char = 'T'; break;
        case RANK_JACK:  rank_char = 'J'; break;
        case RANK_QUEEN: rank_char = 'Q'; break;
        case RANK_KING:  rank_char = 'K'; break;
        case RANK_ACE:   rank_char = 'A'; break;
        default:         return -1;
    }

    // Map suit to character
    char suit_char;
    switch (card.suit) {
        case SUIT_HEARTS:   suit_char = 'h'; break;
        case SUIT_DIAMONDS: suit_char = 'd'; break;
        case SUIT_CLUBS:    suit_char = 'c'; break;
        case SUIT_SPADES:   suit_char = 's'; break;
        default:            return -1;
    }

    // Use snprintf for buffer overflow protection
    snprintf(buffer, size, "%c%c", rank_char, suit_char);
    return 0;
}
