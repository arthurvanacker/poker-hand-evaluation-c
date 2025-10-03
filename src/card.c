/* card.c - Card representation and utilities */

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "../include/poker.h"

void card_init(void) {
    /* Placeholder for card initialization */
}

int card_to_string(const Card card, char* const buffer, const size_t size) {
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

int parse_card(const char* const str, Card* const out_card) {
    // Validate input pointers
    if (str == NULL || out_card == NULL) {
        return -1;
    }

    // Validate string length (must be exactly 2 characters)
    size_t len = strlen(str);
    if (len != 2) {
        return -1;
    }

    // Parse rank character (case-insensitive)
    char rank_char = toupper(str[0]);
    uint8_t rank;

    switch (rank_char) {
        case '2': rank = RANK_TWO;   break;
        case '3': rank = RANK_THREE; break;
        case '4': rank = RANK_FOUR;  break;
        case '5': rank = RANK_FIVE;  break;
        case '6': rank = RANK_SIX;   break;
        case '7': rank = RANK_SEVEN; break;
        case '8': rank = RANK_EIGHT; break;
        case '9': rank = RANK_NINE;  break;
        case 'T': rank = RANK_TEN;   break;
        case 'J': rank = RANK_JACK;  break;
        case 'Q': rank = RANK_QUEEN; break;
        case 'K': rank = RANK_KING;  break;
        case 'A': rank = RANK_ACE;   break;
        default:  return -1;
    }

    // Parse suit character (case-insensitive)
    char suit_char = toupper(str[1]);
    uint8_t suit;

    switch (suit_char) {
        case 'H': suit = SUIT_HEARTS;   break;
        case 'D': suit = SUIT_DIAMONDS; break;
        case 'C': suit = SUIT_CLUBS;    break;
        case 'S': suit = SUIT_SPADES;   break;
        default:  return -1;
    }

    // Set output card
    out_card->rank = rank;
    out_card->suit = suit;

    return 0;
}
