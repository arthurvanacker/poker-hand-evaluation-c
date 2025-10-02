#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "../include/poker.h"

/*
 * Test Suite for Hand Struct
 * Tests verify hand structure creation and properties
 */

void test_hand_size(void) {
    printf("Testing Hand struct size...\n");

    // Test that Hand struct has reasonable size
    size_t hand_size = sizeof(Hand);

    printf("  Hand size: %zu bytes\n", hand_size);

    // Hand should be reasonably sized (less than 100 bytes is acceptable)
    assert(hand_size < 100);

    printf("  ✓ Hand size is reasonable\n");
}

void test_hand_creation_high_card(void) {
    printf("Testing Hand creation with HIGH_CARD...\n");

    // Create a high card hand
    Hand hand;
    hand.cards[0] = (Card){RANK_ACE, SUIT_SPADES};
    hand.cards[1] = (Card){RANK_KING, SUIT_HEARTS};
    hand.cards[2] = (Card){RANK_QUEEN, SUIT_DIAMONDS};
    hand.cards[3] = (Card){RANK_JACK, SUIT_CLUBS};
    hand.cards[4] = (Card){RANK_NINE, SUIT_SPADES};
    hand.category = HAND_HIGH_CARD;
    hand.tiebreakers[0] = RANK_ACE;
    hand.tiebreakers[1] = RANK_KING;
    hand.tiebreakers[2] = RANK_QUEEN;
    hand.tiebreakers[3] = RANK_JACK;
    hand.tiebreakers[4] = RANK_NINE;
    hand.num_tiebreakers = 5;

    // Verify hand properties
    assert(hand.category == HAND_HIGH_CARD);
    assert(hand.cards[0].rank == RANK_ACE);
    assert(hand.cards[0].suit == SUIT_SPADES);
    assert(hand.num_tiebreakers == 5);
    assert(hand.tiebreakers[0] == RANK_ACE);

    printf("  ✓ HIGH_CARD hand created correctly\n");
}

void test_hand_creation_one_pair(void) {
    printf("Testing Hand creation with ONE_PAIR...\n");

    // Create a one pair hand (pair of Kings)
    Hand hand;
    hand.cards[0] = (Card){RANK_KING, SUIT_SPADES};
    hand.cards[1] = (Card){RANK_KING, SUIT_HEARTS};
    hand.cards[2] = (Card){RANK_ACE, SUIT_DIAMONDS};
    hand.cards[3] = (Card){RANK_QUEEN, SUIT_CLUBS};
    hand.cards[4] = (Card){RANK_JACK, SUIT_SPADES};
    hand.category = HAND_ONE_PAIR;
    hand.tiebreakers[0] = RANK_KING;  // Pair rank
    hand.tiebreakers[1] = RANK_ACE;   // Highest kicker
    hand.tiebreakers[2] = RANK_QUEEN; // Second kicker
    hand.tiebreakers[3] = RANK_JACK;  // Third kicker
    hand.num_tiebreakers = 4;

    // Verify hand properties
    assert(hand.category == HAND_ONE_PAIR);
    assert(hand.num_tiebreakers == 4);
    assert(hand.tiebreakers[0] == RANK_KING);
    assert(hand.tiebreakers[1] == RANK_ACE);

    printf("  ✓ ONE_PAIR hand created correctly\n");
}

void test_hand_creation_two_pair(void) {
    printf("Testing Hand creation with TWO_PAIR...\n");

    // Create a two pair hand (Kings and Queens)
    Hand hand;
    hand.cards[0] = (Card){RANK_KING, SUIT_SPADES};
    hand.cards[1] = (Card){RANK_KING, SUIT_HEARTS};
    hand.cards[2] = (Card){RANK_QUEEN, SUIT_DIAMONDS};
    hand.cards[3] = (Card){RANK_QUEEN, SUIT_CLUBS};
    hand.cards[4] = (Card){RANK_ACE, SUIT_SPADES};
    hand.category = HAND_TWO_PAIR;
    hand.tiebreakers[0] = RANK_KING;  // Higher pair
    hand.tiebreakers[1] = RANK_QUEEN; // Lower pair
    hand.tiebreakers[2] = RANK_ACE;   // Kicker
    hand.num_tiebreakers = 3;

    // Verify hand properties
    assert(hand.category == HAND_TWO_PAIR);
    assert(hand.num_tiebreakers == 3);
    assert(hand.tiebreakers[0] == RANK_KING);
    assert(hand.tiebreakers[1] == RANK_QUEEN);
    assert(hand.tiebreakers[2] == RANK_ACE);

    printf("  ✓ TWO_PAIR hand created correctly\n");
}

void test_hand_creation_three_of_a_kind(void) {
    printf("Testing Hand creation with THREE_OF_A_KIND...\n");

    // Create a three of a kind hand (three Kings)
    Hand hand;
    hand.cards[0] = (Card){RANK_KING, SUIT_SPADES};
    hand.cards[1] = (Card){RANK_KING, SUIT_HEARTS};
    hand.cards[2] = (Card){RANK_KING, SUIT_DIAMONDS};
    hand.cards[3] = (Card){RANK_ACE, SUIT_CLUBS};
    hand.cards[4] = (Card){RANK_QUEEN, SUIT_SPADES};
    hand.category = HAND_THREE_OF_A_KIND;
    hand.tiebreakers[0] = RANK_KING;  // Three of a kind rank
    hand.tiebreakers[1] = RANK_ACE;   // Highest kicker
    hand.tiebreakers[2] = RANK_QUEEN; // Second kicker
    hand.num_tiebreakers = 3;

    // Verify hand properties
    assert(hand.category == HAND_THREE_OF_A_KIND);
    assert(hand.num_tiebreakers == 3);
    assert(hand.tiebreakers[0] == RANK_KING);

    printf("  ✓ THREE_OF_A_KIND hand created correctly\n");
}

void test_hand_creation_straight(void) {
    printf("Testing Hand creation with STRAIGHT...\n");

    // Create a straight hand (10-J-Q-K-A)
    Hand hand;
    hand.cards[0] = (Card){RANK_TEN, SUIT_SPADES};
    hand.cards[1] = (Card){RANK_JACK, SUIT_HEARTS};
    hand.cards[2] = (Card){RANK_QUEEN, SUIT_DIAMONDS};
    hand.cards[3] = (Card){RANK_KING, SUIT_CLUBS};
    hand.cards[4] = (Card){RANK_ACE, SUIT_SPADES};
    hand.category = HAND_STRAIGHT;
    hand.tiebreakers[0] = RANK_ACE; // High card of straight
    hand.num_tiebreakers = 1;

    // Verify hand properties
    assert(hand.category == HAND_STRAIGHT);
    assert(hand.num_tiebreakers == 1);
    assert(hand.tiebreakers[0] == RANK_ACE);

    printf("  ✓ STRAIGHT hand created correctly\n");
}

void test_hand_creation_flush(void) {
    printf("Testing Hand creation with FLUSH...\n");

    // Create a flush hand (all hearts)
    Hand hand;
    hand.cards[0] = (Card){RANK_ACE, SUIT_HEARTS};
    hand.cards[1] = (Card){RANK_KING, SUIT_HEARTS};
    hand.cards[2] = (Card){RANK_QUEEN, SUIT_HEARTS};
    hand.cards[3] = (Card){RANK_JACK, SUIT_HEARTS};
    hand.cards[4] = (Card){RANK_NINE, SUIT_HEARTS};
    hand.category = HAND_FLUSH;
    hand.tiebreakers[0] = RANK_ACE;
    hand.tiebreakers[1] = RANK_KING;
    hand.tiebreakers[2] = RANK_QUEEN;
    hand.tiebreakers[3] = RANK_JACK;
    hand.tiebreakers[4] = RANK_NINE;
    hand.num_tiebreakers = 5;

    // Verify hand properties
    assert(hand.category == HAND_FLUSH);
    assert(hand.num_tiebreakers == 5);
    assert(hand.cards[0].suit == SUIT_HEARTS);
    assert(hand.cards[1].suit == SUIT_HEARTS);
    assert(hand.cards[2].suit == SUIT_HEARTS);
    assert(hand.cards[3].suit == SUIT_HEARTS);
    assert(hand.cards[4].suit == SUIT_HEARTS);

    printf("  ✓ FLUSH hand created correctly\n");
}

void test_hand_creation_full_house(void) {
    printf("Testing Hand creation with FULL_HOUSE...\n");

    // Create a full house hand (three Kings, two Aces)
    Hand hand;
    hand.cards[0] = (Card){RANK_KING, SUIT_SPADES};
    hand.cards[1] = (Card){RANK_KING, SUIT_HEARTS};
    hand.cards[2] = (Card){RANK_KING, SUIT_DIAMONDS};
    hand.cards[3] = (Card){RANK_ACE, SUIT_CLUBS};
    hand.cards[4] = (Card){RANK_ACE, SUIT_SPADES};
    hand.category = HAND_FULL_HOUSE;
    hand.tiebreakers[0] = RANK_KING; // Three of a kind rank
    hand.tiebreakers[1] = RANK_ACE;  // Pair rank
    hand.num_tiebreakers = 2;

    // Verify hand properties
    assert(hand.category == HAND_FULL_HOUSE);
    assert(hand.num_tiebreakers == 2);
    assert(hand.tiebreakers[0] == RANK_KING);
    assert(hand.tiebreakers[1] == RANK_ACE);

    printf("  ✓ FULL_HOUSE hand created correctly\n");
}

void test_hand_creation_four_of_a_kind(void) {
    printf("Testing Hand creation with FOUR_OF_A_KIND...\n");

    // Create a four of a kind hand (four Kings)
    Hand hand;
    hand.cards[0] = (Card){RANK_KING, SUIT_SPADES};
    hand.cards[1] = (Card){RANK_KING, SUIT_HEARTS};
    hand.cards[2] = (Card){RANK_KING, SUIT_DIAMONDS};
    hand.cards[3] = (Card){RANK_KING, SUIT_CLUBS};
    hand.cards[4] = (Card){RANK_ACE, SUIT_SPADES};
    hand.category = HAND_FOUR_OF_A_KIND;
    hand.tiebreakers[0] = RANK_KING; // Four of a kind rank
    hand.tiebreakers[1] = RANK_ACE;  // Kicker
    hand.num_tiebreakers = 2;

    // Verify hand properties
    assert(hand.category == HAND_FOUR_OF_A_KIND);
    assert(hand.num_tiebreakers == 2);
    assert(hand.tiebreakers[0] == RANK_KING);
    assert(hand.tiebreakers[1] == RANK_ACE);

    printf("  ✓ FOUR_OF_A_KIND hand created correctly\n");
}

void test_hand_creation_straight_flush(void) {
    printf("Testing Hand creation with STRAIGHT_FLUSH...\n");

    // Create a straight flush hand (5-6-7-8-9 of hearts)
    Hand hand;
    hand.cards[0] = (Card){RANK_FIVE, SUIT_HEARTS};
    hand.cards[1] = (Card){RANK_SIX, SUIT_HEARTS};
    hand.cards[2] = (Card){RANK_SEVEN, SUIT_HEARTS};
    hand.cards[3] = (Card){RANK_EIGHT, SUIT_HEARTS};
    hand.cards[4] = (Card){RANK_NINE, SUIT_HEARTS};
    hand.category = HAND_STRAIGHT_FLUSH;
    hand.tiebreakers[0] = RANK_NINE; // High card of straight flush
    hand.num_tiebreakers = 1;

    // Verify hand properties
    assert(hand.category == HAND_STRAIGHT_FLUSH);
    assert(hand.num_tiebreakers == 1);
    assert(hand.tiebreakers[0] == RANK_NINE);
    assert(hand.cards[0].suit == SUIT_HEARTS);
    assert(hand.cards[1].suit == SUIT_HEARTS);

    printf("  ✓ STRAIGHT_FLUSH hand created correctly\n");
}

void test_hand_creation_royal_flush(void) {
    printf("Testing Hand creation with ROYAL_FLUSH...\n");

    // Create a royal flush hand (10-J-Q-K-A of spades)
    Hand hand;
    hand.cards[0] = (Card){RANK_TEN, SUIT_SPADES};
    hand.cards[1] = (Card){RANK_JACK, SUIT_SPADES};
    hand.cards[2] = (Card){RANK_QUEEN, SUIT_SPADES};
    hand.cards[3] = (Card){RANK_KING, SUIT_SPADES};
    hand.cards[4] = (Card){RANK_ACE, SUIT_SPADES};
    hand.category = HAND_ROYAL_FLUSH;
    hand.tiebreakers[0] = RANK_ACE; // High card (always Ace for royal flush)
    hand.num_tiebreakers = 1;

    // Verify hand properties
    assert(hand.category == HAND_ROYAL_FLUSH);
    assert(hand.num_tiebreakers == 1);
    assert(hand.tiebreakers[0] == RANK_ACE);
    assert(hand.cards[4].rank == RANK_ACE);
    assert(hand.cards[4].suit == SUIT_SPADES);

    printf("  ✓ ROYAL_FLUSH hand created correctly\n");
}

void test_hand_cards_array(void) {
    printf("Testing Hand cards array size...\n");

    // Verify cards array is exactly 5 elements
    Hand hand;
    size_t cards_array_size = sizeof(hand.cards) / sizeof(hand.cards[0]);

    assert(cards_array_size == 5);

    printf("  ✓ Cards array has exactly 5 elements\n");
}

void test_hand_tiebreakers_array(void) {
    printf("Testing Hand tiebreakers array size...\n");

    // Verify tiebreakers array is exactly MAX_TIEBREAKERS elements
    Hand hand;
    size_t tiebreakers_array_size = sizeof(hand.tiebreakers) / sizeof(hand.tiebreakers[0]);

    assert(tiebreakers_array_size == MAX_TIEBREAKERS);
    assert(MAX_TIEBREAKERS == 5);

    printf("  ✓ Tiebreakers array has exactly MAX_TIEBREAKERS elements\n");
}

void test_hand_empty_tiebreakers(void) {
    printf("Testing Hand with no tiebreakers...\n");

    // Create a hand with no tiebreakers (edge case)
    Hand hand;
    hand.cards[0] = (Card){RANK_ACE, SUIT_SPADES};
    hand.cards[1] = (Card){RANK_KING, SUIT_HEARTS};
    hand.cards[2] = (Card){RANK_QUEEN, SUIT_DIAMONDS};
    hand.cards[3] = (Card){RANK_JACK, SUIT_CLUBS};
    hand.cards[4] = (Card){RANK_TEN, SUIT_SPADES};
    hand.category = HAND_ROYAL_FLUSH;
    hand.num_tiebreakers = 0; // No tiebreakers

    // Verify hand properties
    assert(hand.category == HAND_ROYAL_FLUSH);
    assert(hand.num_tiebreakers == 0);

    printf("  ✓ Hand with no tiebreakers works correctly\n");
}

void test_max_tiebreakers_constant(void) {
    printf("Testing MAX_TIEBREAKERS constant...\n");

    // Verify MAX_TIEBREAKERS is defined and equals 5
    assert(MAX_TIEBREAKERS == 5);

    printf("  ✓ MAX_TIEBREAKERS is defined as 5\n");
}

int main(void) {
    printf("\n=== Hand Struct Test Suite ===\n\n");

    test_hand_size();
    test_hand_cards_array();
    test_hand_tiebreakers_array();
    test_max_tiebreakers_constant();
    test_hand_empty_tiebreakers();

    printf("\n=== Hand Creation by Category Test Suite ===\n\n");

    test_hand_creation_high_card();
    test_hand_creation_one_pair();
    test_hand_creation_two_pair();
    test_hand_creation_three_of_a_kind();
    test_hand_creation_straight();
    test_hand_creation_flush();
    test_hand_creation_full_house();
    test_hand_creation_four_of_a_kind();
    test_hand_creation_straight_flush();
    test_hand_creation_royal_flush();

    printf("\n=== All tests passed! ===\n\n");
    return 0;
}
