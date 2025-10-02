#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "../include/poker.h"

/*
 * Test Suite for HandCategory Enum and Hand Struct
 * Tests verify hand category values and hand structure
 */

void test_hand_category_values(void) {
    printf("Testing HandCategory enum values...\n");

    // Test that all hand categories are defined with correct values
    assert(HAND_HIGH_CARD == 1);
    assert(HAND_ONE_PAIR == 2);
    assert(HAND_TWO_PAIR == 3);
    assert(HAND_THREE_OF_A_KIND == 4);
    assert(HAND_STRAIGHT == 5);
    assert(HAND_FLUSH == 6);
    assert(HAND_FULL_HOUSE == 7);
    assert(HAND_FOUR_OF_A_KIND == 8);
    assert(HAND_STRAIGHT_FLUSH == 9);
    assert(HAND_ROYAL_FLUSH == 10);

    printf("  ✓ All hand category values correct\n");
}

void test_hand_category_distinct(void) {
    printf("Testing HandCategory distinctness...\n");

    // Test that all categories are different from each other
    HandCategory categories[] = {
        HAND_HIGH_CARD,
        HAND_ONE_PAIR,
        HAND_TWO_PAIR,
        HAND_THREE_OF_A_KIND,
        HAND_STRAIGHT,
        HAND_FLUSH,
        HAND_FULL_HOUSE,
        HAND_FOUR_OF_A_KIND,
        HAND_STRAIGHT_FLUSH,
        HAND_ROYAL_FLUSH
    };

    // Verify all 10 categories are distinct
    for (int i = 0; i < 10; i++) {
        for (int j = i + 1; j < 10; j++) {
            assert(categories[i] != categories[j]);
        }
    }

    printf("  ✓ All hand categories are distinct\n");
}

void test_hand_category_ordered(void) {
    printf("Testing HandCategory ordering (weakest to strongest)...\n");

    // Test that categories are ordered from weakest to strongest
    assert(HAND_HIGH_CARD < HAND_ONE_PAIR);
    assert(HAND_ONE_PAIR < HAND_TWO_PAIR);
    assert(HAND_TWO_PAIR < HAND_THREE_OF_A_KIND);
    assert(HAND_THREE_OF_A_KIND < HAND_STRAIGHT);
    assert(HAND_STRAIGHT < HAND_FLUSH);
    assert(HAND_FLUSH < HAND_FULL_HOUSE);
    assert(HAND_FULL_HOUSE < HAND_FOUR_OF_A_KIND);
    assert(HAND_FOUR_OF_A_KIND < HAND_STRAIGHT_FLUSH);
    assert(HAND_STRAIGHT_FLUSH < HAND_ROYAL_FLUSH);

    printf("  ✓ Hand categories are properly ordered\n");
}

void test_hand_struct_size(void) {
    printf("Testing Hand struct size...\n");

    // Test that Hand struct has reasonable size
    size_t hand_size = sizeof(Hand);
    printf("  Hand size: %zu bytes\n", hand_size);

    // Hand should contain: 5 Cards (10 bytes) + category (4 bytes)
    // + 5 tiebreakers (5 bytes) + num_tiebreakers (8 bytes) = ~27-32 bytes
    assert(hand_size > 0);
    assert(hand_size <= 64); // Reasonable upper bound

    printf("  ✓ Hand size is reasonable\n");
}

void test_hand_creation_with_high_card(void) {
    printf("Testing Hand creation with high card category...\n");

    // Create a hand with 5 cards
    Hand hand;
    hand.cards[0] = (Card){RANK_ACE, SUIT_HEARTS};
    hand.cards[1] = (Card){RANK_KING, SUIT_DIAMONDS};
    hand.cards[2] = (Card){RANK_QUEEN, SUIT_CLUBS};
    hand.cards[3] = (Card){RANK_JACK, SUIT_SPADES};
    hand.cards[4] = (Card){RANK_NINE, SUIT_HEARTS};
    hand.category = HAND_HIGH_CARD;
    hand.tiebreakers[0] = RANK_ACE;
    hand.tiebreakers[1] = RANK_KING;
    hand.tiebreakers[2] = RANK_QUEEN;
    hand.tiebreakers[3] = RANK_JACK;
    hand.tiebreakers[4] = RANK_NINE;
    hand.num_tiebreakers = 5;

    // Verify hand structure
    assert(hand.cards[0].rank == RANK_ACE);
    assert(hand.cards[0].suit == SUIT_HEARTS);
    assert(hand.category == HAND_HIGH_CARD);
    assert(hand.tiebreakers[0] == RANK_ACE);
    assert(hand.num_tiebreakers == 5);

    printf("  ✓ Hand with high card category created correctly\n");
}

void test_hand_creation_with_one_pair(void) {
    printf("Testing Hand creation with one pair category...\n");

    Hand hand;
    hand.cards[0] = (Card){RANK_ACE, SUIT_HEARTS};
    hand.cards[1] = (Card){RANK_ACE, SUIT_DIAMONDS};
    hand.cards[2] = (Card){RANK_KING, SUIT_CLUBS};
    hand.cards[3] = (Card){RANK_QUEEN, SUIT_SPADES};
    hand.cards[4] = (Card){RANK_JACK, SUIT_HEARTS};
    hand.category = HAND_ONE_PAIR;
    hand.tiebreakers[0] = RANK_ACE;  // Pair rank
    hand.tiebreakers[1] = RANK_KING; // Kicker 1
    hand.tiebreakers[2] = RANK_QUEEN; // Kicker 2
    hand.tiebreakers[3] = RANK_JACK; // Kicker 3
    hand.num_tiebreakers = 4;

    assert(hand.category == HAND_ONE_PAIR);
    assert(hand.tiebreakers[0] == RANK_ACE);
    assert(hand.num_tiebreakers == 4);

    printf("  ✓ Hand with one pair category created correctly\n");
}

void test_hand_creation_with_royal_flush(void) {
    printf("Testing Hand creation with royal flush category...\n");

    Hand hand;
    hand.cards[0] = (Card){RANK_ACE, SUIT_SPADES};
    hand.cards[1] = (Card){RANK_KING, SUIT_SPADES};
    hand.cards[2] = (Card){RANK_QUEEN, SUIT_SPADES};
    hand.cards[3] = (Card){RANK_JACK, SUIT_SPADES};
    hand.cards[4] = (Card){RANK_TEN, SUIT_SPADES};
    hand.category = HAND_ROYAL_FLUSH;
    hand.num_tiebreakers = 0; // Royal flush needs no tiebreakers

    assert(hand.category == HAND_ROYAL_FLUSH);
    assert(hand.cards[0].rank == RANK_ACE);
    assert(hand.cards[0].suit == SUIT_SPADES);
    assert(hand.num_tiebreakers == 0);

    printf("  ✓ Hand with royal flush category created correctly\n");
}

void test_hand_exactly_five_cards(void) {
    printf("Testing Hand contains exactly 5 cards...\n");

    Hand hand;

    // Verify array size is exactly 5
    size_t num_cards = sizeof(hand.cards) / sizeof(hand.cards[0]);
    assert(num_cards == 5);

    printf("  ✓ Hand contains exactly 5 cards\n");
}

void test_hand_max_tiebreakers(void) {
    printf("Testing Hand MAX_TIEBREAKERS constant...\n");

    // Verify MAX_TIEBREAKERS is defined and equals 5
    assert(MAX_TIEBREAKERS == 5);

    // Verify tiebreakers array has MAX_TIEBREAKERS elements
    Hand hand;
    size_t num_tiebreakers = sizeof(hand.tiebreakers) / sizeof(hand.tiebreakers[0]);
    assert(num_tiebreakers == MAX_TIEBREAKERS);

    printf("  ✓ MAX_TIEBREAKERS is 5 and tiebreakers array size is correct\n");
}

void test_hand_tiebreakers_descending_order(void) {
    printf("Testing Hand tiebreakers in descending importance order...\n");

    Hand hand;
    hand.category = HAND_HIGH_CARD;
    hand.tiebreakers[0] = RANK_ACE;   // Most important
    hand.tiebreakers[1] = RANK_KING;
    hand.tiebreakers[2] = RANK_QUEEN;
    hand.tiebreakers[3] = RANK_JACK;
    hand.tiebreakers[4] = RANK_TEN;   // Least important
    hand.num_tiebreakers = 5;

    // Verify descending order
    assert(hand.tiebreakers[0] > hand.tiebreakers[1]);
    assert(hand.tiebreakers[1] > hand.tiebreakers[2]);
    assert(hand.tiebreakers[2] > hand.tiebreakers[3]);
    assert(hand.tiebreakers[3] > hand.tiebreakers[4]);

    printf("  ✓ Tiebreakers stored in descending importance order\n");
}

int main(void) {
    printf("\n=== HandCategory Enum Test Suite ===\n\n");

    test_hand_category_values();
    test_hand_category_distinct();
    test_hand_category_ordered();

    printf("\n=== Hand Struct Test Suite ===\n\n");

    test_hand_struct_size();
    test_hand_exactly_five_cards();
    test_hand_max_tiebreakers();
    test_hand_creation_with_high_card();
    test_hand_creation_with_one_pair();
    test_hand_creation_with_royal_flush();
    test_hand_tiebreakers_descending_order();

    printf("\n=== All tests passed! ===\n\n");
    return 0;
}
