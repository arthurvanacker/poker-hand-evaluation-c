#include <assert.h>
#include <stdio.h>
#include "../include/poker.h"

/*
 * Test Suite for Suit Enum and Card Struct
 * Tests verify suit values and card representation
 */

void test_suit_values(void) {
    printf("Testing suit values...\n");

    // Test that all suit values are defined and distinct
    assert(SUIT_HEARTS == 0);
    assert(SUIT_DIAMONDS == 1);
    assert(SUIT_CLUBS == 2);
    assert(SUIT_SPADES == 3);

    printf("  ✓ All suit values correct\n");
}

void test_suit_distinct(void) {
    printf("Testing suit distinctness...\n");

    // Test that all suits are different from each other
    assert(SUIT_HEARTS != SUIT_DIAMONDS);
    assert(SUIT_HEARTS != SUIT_CLUBS);
    assert(SUIT_HEARTS != SUIT_SPADES);
    assert(SUIT_DIAMONDS != SUIT_CLUBS);
    assert(SUIT_DIAMONDS != SUIT_SPADES);
    assert(SUIT_CLUBS != SUIT_SPADES);

    printf("  ✓ All suits are distinct\n");
}

void test_card_creation(void) {
    printf("Testing card creation...\n");

    // Test creating a card with rank and suit
    Card card;
    card.rank = RANK_ACE;
    card.suit = SUIT_SPADES;

    assert(card.rank == RANK_ACE);
    assert(card.suit == SUIT_SPADES);

    printf("  ✓ Card creation works correctly\n");
}

void test_card_size(void) {
    printf("Testing card size...\n");

    // Test that Card struct is efficiently sized (2 bytes expected)
    // Each enum should be 1 byte if properly sized
    size_t card_size = sizeof(Card);

    printf("  Card size: %zu bytes\n", card_size);

    // Card should be small and efficient (2-4 bytes is acceptable)
    assert(card_size <= 4);

    printf("  ✓ Card size is efficient\n");
}

void test_all_52_cards(void) {
    printf("Testing all 52 unique cards...\n");

    // Test that all 52 unique cards can be represented
    Rank ranks[] = {
        RANK_TWO, RANK_THREE, RANK_FOUR, RANK_FIVE, RANK_SIX,
        RANK_SEVEN, RANK_EIGHT, RANK_NINE, RANK_TEN, RANK_JACK,
        RANK_QUEEN, RANK_KING, RANK_ACE
    };

    Suit suits[] = {
        SUIT_HEARTS, SUIT_DIAMONDS, SUIT_CLUBS, SUIT_SPADES
    };

    int card_count = 0;

    // Create all combinations of rank and suit
    for (int r = 0; r < 13; r++) {
        for (int s = 0; s < 4; s++) {
            Card card;
            card.rank = ranks[r];
            card.suit = suits[s];

            // Verify the card was created correctly
            assert(card.rank == ranks[r]);
            assert(card.suit == suits[s]);

            card_count++;
        }
    }

    // Verify we created exactly 52 cards
    assert(card_count == 52);

    printf("  ✓ All 52 unique cards can be represented\n");
}

void test_card_combinations(void) {
    printf("Testing specific card combinations...\n");

    // Test some specific well-known cards
    Card ace_of_spades;
    ace_of_spades.rank = RANK_ACE;
    ace_of_spades.suit = SUIT_SPADES;
    assert(ace_of_spades.rank == RANK_ACE);
    assert(ace_of_spades.suit == SUIT_SPADES);

    Card two_of_hearts;
    two_of_hearts.rank = RANK_TWO;
    two_of_hearts.suit = SUIT_HEARTS;
    assert(two_of_hearts.rank == RANK_TWO);
    assert(two_of_hearts.suit == SUIT_HEARTS);

    Card king_of_diamonds;
    king_of_diamonds.rank = RANK_KING;
    king_of_diamonds.suit = SUIT_DIAMONDS;
    assert(king_of_diamonds.rank == RANK_KING);
    assert(king_of_diamonds.suit == SUIT_DIAMONDS);

    printf("  ✓ Specific card combinations work correctly\n");
}

int main(void) {
    printf("\n=== Card Struct Test Suite ===\n\n");

    test_suit_values();
    test_suit_distinct();
    test_card_creation();
    test_card_size();
    test_all_52_cards();
    test_card_combinations();

    printf("\n=== All tests passed! ===\n\n");
    return 0;
}
