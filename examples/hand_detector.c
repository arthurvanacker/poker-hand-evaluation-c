/*
 * Hand Detector Example
 *
 * Demonstrates all 10 hand category detector functions with specific examples.
 * Shows how to use each detector and interpret the results.
 *
 * Compile:
 *   gcc -Wall -Wextra -std=c99 -Iinclude examples/hand_detector.c lib/libpoker.a -o examples/hand_detector
 *
 * Run:
 *   ./examples/hand_detector
 */

#include "../include/poker.h"
#include <stdio.h>
#include <string.h>

/* Helper function to print a card */
static void print_card(const Card card) {
    char buffer[3];
    if (card_to_string(card, buffer, sizeof(buffer)) == 0) {
        printf("%s", buffer);
    } else {
        printf("??");
    }
}

/* Helper function to print a hand */
static void print_hand(const Card* const cards, const size_t len) {
    for (size_t i = 0; i < len; i++) {
        print_card(cards[i]);
        if (i < len - 1) {
            printf(" ");
        }
    }
}

/* Helper function to print tiebreakers */
static void print_tiebreakers(const Rank* const tiebreakers, const size_t num) {
    if (num == 0) {
        return;
    }

    printf(" (Tiebreakers: ");
    for (size_t i = 0; i < num; i++) {
        printf("%d", tiebreakers[i]);
        if (i < num - 1) {
            printf(", ");
        }
    }
    printf(")");
}

/* Test royal flush */
static void test_royal_flush(void) {
    printf("\n=== Royal Flush ===\n");

    const Card hand[HAND_SIZE] = {
        {RANK_ACE, SUIT_HEARTS},
        {RANK_KING, SUIT_HEARTS},
        {RANK_QUEEN, SUIT_HEARTS},
        {RANK_JACK, SUIT_HEARTS},
        {RANK_TEN, SUIT_HEARTS}
    };

    printf("Hand: ");
    print_hand(hand, HAND_SIZE);

    if (detect_royal_flush(hand, HAND_SIZE)) {
        printf(" -> ROYAL FLUSH!\n");
    } else {
        printf(" -> Not a royal flush (unexpected!)\n");
    }
}

/* Test straight flush */
static void test_straight_flush(void) {
    printf("\n=== Straight Flush ===\n");

    const Card hand[HAND_SIZE] = {
        {RANK_NINE, SUIT_DIAMONDS},
        {RANK_EIGHT, SUIT_DIAMONDS},
        {RANK_SEVEN, SUIT_DIAMONDS},
        {RANK_SIX, SUIT_DIAMONDS},
        {RANK_FIVE, SUIT_DIAMONDS}
    };

    printf("Hand: ");
    print_hand(hand, HAND_SIZE);

    Rank high_card;
    if (detect_straight_flush(hand, HAND_SIZE, &high_card)) {
        printf(" -> STRAIGHT FLUSH (high card: %d)!\n", high_card);
    } else {
        printf(" -> Not a straight flush (unexpected!)\n");
    }
}

/* Test four of a kind */
static void test_four_of_a_kind(void) {
    printf("\n=== Four of a Kind ===\n");

    const Card hand[HAND_SIZE] = {
        {RANK_KING, SUIT_HEARTS},
        {RANK_KING, SUIT_DIAMONDS},
        {RANK_KING, SUIT_CLUBS},
        {RANK_KING, SUIT_SPADES},
        {RANK_TWO, SUIT_HEARTS}
    };

    printf("Hand: ");
    print_hand(hand, HAND_SIZE);

    Rank tiebreakers[MAX_TIEBREAKERS];
    size_t num_tiebreakers;

    if (detect_four_of_a_kind(hand, HAND_SIZE, NULL, tiebreakers, &num_tiebreakers)) {
        printf(" -> FOUR OF A KIND");
        print_tiebreakers(tiebreakers, num_tiebreakers);
        printf("!\n");
    } else {
        printf(" -> Not four of a kind (unexpected!)\n");
    }
}

/* Test full house */
static void test_full_house(void) {
    printf("\n=== Full House ===\n");

    const Card hand[HAND_SIZE] = {
        {RANK_ACE, SUIT_HEARTS},
        {RANK_ACE, SUIT_DIAMONDS},
        {RANK_ACE, SUIT_CLUBS},
        {RANK_KING, SUIT_SPADES},
        {RANK_KING, SUIT_HEARTS}
    };

    printf("Hand: ");
    print_hand(hand, HAND_SIZE);

    Rank tiebreakers[MAX_TIEBREAKERS];
    size_t num_tiebreakers;

    if (detect_full_house(hand, HAND_SIZE, NULL, tiebreakers, &num_tiebreakers)) {
        printf(" -> FULL HOUSE");
        print_tiebreakers(tiebreakers, num_tiebreakers);
        printf("!\n");
    } else {
        printf(" -> Not a full house (unexpected!)\n");
    }
}

/* Test flush */
static void test_flush(void) {
    printf("\n=== Flush ===\n");

    const Card hand[HAND_SIZE] = {
        {RANK_ACE, SUIT_CLUBS},
        {RANK_JACK, SUIT_CLUBS},
        {RANK_NINE, SUIT_CLUBS},
        {RANK_SEVEN, SUIT_CLUBS},
        {RANK_THREE, SUIT_CLUBS}
    };

    printf("Hand: ");
    print_hand(hand, HAND_SIZE);

    Rank tiebreakers[MAX_TIEBREAKERS];
    size_t num_tiebreakers;

    if (detect_flush(hand, HAND_SIZE, tiebreakers, &num_tiebreakers)) {
        printf(" -> FLUSH");
        print_tiebreakers(tiebreakers, num_tiebreakers);
        printf("!\n");
    } else {
        printf(" -> Not a flush (unexpected!)\n");
    }
}

/* Test straight */
static void test_straight(void) {
    printf("\n=== Straight ===\n");

    const Card hand[HAND_SIZE] = {
        {RANK_NINE, SUIT_HEARTS},
        {RANK_EIGHT, SUIT_DIAMONDS},
        {RANK_SEVEN, SUIT_CLUBS},
        {RANK_SIX, SUIT_SPADES},
        {RANK_FIVE, SUIT_HEARTS}
    };

    printf("Hand: ");
    print_hand(hand, HAND_SIZE);

    Rank tiebreakers[MAX_TIEBREAKERS];
    size_t num_tiebreakers;

    if (detect_straight(hand, HAND_SIZE, tiebreakers, &num_tiebreakers)) {
        printf(" -> STRAIGHT");
        print_tiebreakers(tiebreakers, num_tiebreakers);
        printf("!\n");
    } else {
        printf(" -> Not a straight (unexpected!)\n");
    }
}

/* Test three of a kind */
static void test_three_of_a_kind(void) {
    printf("\n=== Three of a Kind ===\n");

    const Card hand[HAND_SIZE] = {
        {RANK_QUEEN, SUIT_HEARTS},
        {RANK_QUEEN, SUIT_DIAMONDS},
        {RANK_QUEEN, SUIT_CLUBS},
        {RANK_EIGHT, SUIT_SPADES},
        {RANK_THREE, SUIT_HEARTS}
    };

    printf("Hand: ");
    print_hand(hand, HAND_SIZE);

    Rank tiebreakers[MAX_TIEBREAKERS];
    size_t num_tiebreakers;

    if (detect_three_of_a_kind(hand, HAND_SIZE, NULL, tiebreakers, &num_tiebreakers)) {
        printf(" -> THREE OF A KIND");
        print_tiebreakers(tiebreakers, num_tiebreakers);
        printf("!\n");
    } else {
        printf(" -> Not three of a kind (unexpected!)\n");
    }
}

/* Test two pair */
static void test_two_pair(void) {
    printf("\n=== Two Pair ===\n");

    const Card hand[HAND_SIZE] = {
        {RANK_JACK, SUIT_HEARTS},
        {RANK_JACK, SUIT_DIAMONDS},
        {RANK_FIVE, SUIT_CLUBS},
        {RANK_FIVE, SUIT_SPADES},
        {RANK_TWO, SUIT_HEARTS}
    };

    printf("Hand: ");
    print_hand(hand, HAND_SIZE);

    Rank tiebreakers[MAX_TIEBREAKERS];
    size_t num_tiebreakers;

    if (detect_two_pair(hand, HAND_SIZE, NULL, tiebreakers, &num_tiebreakers)) {
        printf(" -> TWO PAIR");
        print_tiebreakers(tiebreakers, num_tiebreakers);
        printf("!\n");
    } else {
        printf(" -> Not two pair (unexpected!)\n");
    }
}

/* Test one pair */
static void test_one_pair(void) {
    printf("\n=== One Pair ===\n");

    const Card hand[HAND_SIZE] = {
        {RANK_TEN, SUIT_HEARTS},
        {RANK_TEN, SUIT_DIAMONDS},
        {RANK_SEVEN, SUIT_CLUBS},
        {RANK_FOUR, SUIT_SPADES},
        {RANK_TWO, SUIT_HEARTS}
    };

    printf("Hand: ");
    print_hand(hand, HAND_SIZE);

    Rank tiebreakers[MAX_TIEBREAKERS];
    size_t num_tiebreakers;

    if (detect_one_pair(hand, HAND_SIZE, NULL, tiebreakers, &num_tiebreakers)) {
        printf(" -> ONE PAIR");
        print_tiebreakers(tiebreakers, num_tiebreakers);
        printf("!\n");
    } else {
        printf(" -> Not one pair (unexpected!)\n");
    }
}

/* Test high card */
static void test_high_card(void) {
    printf("\n=== High Card ===\n");

    const Card hand[HAND_SIZE] = {
        {RANK_ACE, SUIT_HEARTS},
        {RANK_JACK, SUIT_DIAMONDS},
        {RANK_EIGHT, SUIT_CLUBS},
        {RANK_FIVE, SUIT_SPADES},
        {RANK_TWO, SUIT_HEARTS}
    };

    printf("Hand: ");
    print_hand(hand, HAND_SIZE);

    Rank tiebreakers[MAX_TIEBREAKERS];
    size_t num_tiebreakers;

    if (detect_high_card(hand, HAND_SIZE, tiebreakers, &num_tiebreakers)) {
        printf(" -> HIGH CARD");
        print_tiebreakers(tiebreakers, num_tiebreakers);
        printf("!\n");
    } else {
        printf(" -> Not valid (unexpected!)\n");
    }
}

int main(void) {
    printf("=== Hand Detector Examples ===\n");
    printf("Demonstrates all 10 poker hand categories\n");

    test_royal_flush();
    test_straight_flush();
    test_four_of_a_kind();
    test_full_house();
    test_flush();
    test_straight();
    test_three_of_a_kind();
    test_two_pair();
    test_one_pair();
    test_high_card();

    printf("\n=== All examples completed! ===\n");

    return 0;
}
