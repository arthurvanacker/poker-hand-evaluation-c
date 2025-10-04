/*
 * Simple Poker Game Example
 *
 * Demonstrates basic library usage:
 * - Creating and shuffling a deck
 * - Dealing cards
 * - Detecting hand categories using detector functions
 * - Converting cards to string representation
 *
 * Compile:
 *   gcc -Wall -Wextra -std=c99 -Iinclude examples/poker_game.c lib/libpoker.a -o examples/poker_game
 *
 * Run:
 *   ./examples/poker_game
 */

#include "../include/poker.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* Helper function to get hand category name */
static const char* hand_category_name(const HandCategory category) {
    switch (category) {
        case HAND_HIGH_CARD:       return "High Card";
        case HAND_ONE_PAIR:        return "One Pair";
        case HAND_TWO_PAIR:        return "Two Pair";
        case HAND_THREE_OF_A_KIND: return "Three of a Kind";
        case HAND_STRAIGHT:        return "Straight";
        case HAND_FLUSH:           return "Flush";
        case HAND_FULL_HOUSE:      return "Full House";
        case HAND_FOUR_OF_A_KIND:  return "Four of a Kind";
        case HAND_STRAIGHT_FLUSH:  return "Straight Flush";
        case HAND_ROYAL_FLUSH:     return "Royal Flush";
        default:                   return "Unknown";
    }
}

/* Detect hand category using available detector functions */
static HandCategory detect_hand_category(const Card* const cards) {
    Rank tiebreakers[MAX_TIEBREAKERS];
    size_t num_tiebreakers;

    /* Check from strongest to weakest */
    if (detect_royal_flush(cards, HAND_SIZE)) {
        return HAND_ROYAL_FLUSH;
    }
    if (detect_straight_flush(cards, HAND_SIZE, NULL)) {
        return HAND_STRAIGHT_FLUSH;
    }
    if (detect_four_of_a_kind(cards, HAND_SIZE, NULL, tiebreakers, &num_tiebreakers)) {
        return HAND_FOUR_OF_A_KIND;
    }
    if (detect_full_house(cards, HAND_SIZE, NULL, tiebreakers, &num_tiebreakers)) {
        return HAND_FULL_HOUSE;
    }
    if (detect_flush(cards, HAND_SIZE, tiebreakers, &num_tiebreakers)) {
        return HAND_FLUSH;
    }
    if (detect_straight(cards, HAND_SIZE, tiebreakers, &num_tiebreakers)) {
        return HAND_STRAIGHT;
    }
    if (detect_three_of_a_kind(cards, HAND_SIZE, NULL, tiebreakers, &num_tiebreakers)) {
        return HAND_THREE_OF_A_KIND;
    }
    if (detect_two_pair(cards, HAND_SIZE, NULL, tiebreakers, &num_tiebreakers)) {
        return HAND_TWO_PAIR;
    }
    if (detect_one_pair(cards, HAND_SIZE, NULL, tiebreakers, &num_tiebreakers)) {
        return HAND_ONE_PAIR;
    }

    /* Default to high card */
    detect_high_card(cards, HAND_SIZE, tiebreakers, &num_tiebreakers);
    return HAND_HIGH_CARD;
}

/* Print a hand of cards */
static void print_hand(const Card* const cards, const size_t len) {
    char buffer[3];
    for (size_t i = 0; i < len; i++) {
        if (card_to_string(cards[i], buffer, sizeof(buffer)) == 0) {
            printf("%s ", buffer);
        } else {
            printf("?? ");
        }
    }
}

int main(void) {
    /* Seed random number generator */
    srand((unsigned int)time(NULL));

    printf("=== Simple Poker Game ===\n\n");

    /* Create new deck */
    Deck* deck = deck_new();
    if (deck == NULL) {
        fprintf(stderr, "Error: Failed to create deck\n");
        return 1;
    }

    printf("Created deck with %zu cards\n", deck->size);

    /* Shuffle the deck */
    deck_shuffle(deck);
    printf("Shuffled deck\n\n");

    /* Deal a 5-card hand */
    Card hand[HAND_SIZE];
    const size_t dealt = deck_deal(deck, hand, HAND_SIZE);

    if (dealt != HAND_SIZE) {
        fprintf(stderr, "Error: Expected to deal %d cards, got %zu\n", HAND_SIZE, dealt);
        deck_free(deck);
        return 1;
    }

    /* Display the hand */
    printf("Your hand: ");
    print_hand(hand, HAND_SIZE);
    printf("\n");

    /* Detect hand category */
    const HandCategory category = detect_hand_category(hand);
    printf("Hand rank: %s\n", hand_category_name(category));

    printf("\nCards remaining in deck: %zu\n", deck->size);

    /* Clean up */
    deck_free(deck);

    return 0;
}
