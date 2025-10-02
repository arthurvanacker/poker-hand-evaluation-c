#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "../include/poker.h"

/*
 * Test Suite for Deck struct and deck_new function
 * Tests verify deck creation, card allocation, and uniqueness
 */

void test_deck_new_allocation(void) {
    printf("Testing deck_new allocation...\n");

    // Test that deck_new returns non-NULL pointer
    Deck* deck = deck_new();
    assert(deck != NULL);
    assert(deck->cards != NULL);

    // Clean up
    deck_free(deck);

    printf("  ✓ deck_new allocates successfully\n");
}

void test_deck_new_size(void) {
    printf("Testing deck size...\n");

    Deck* deck = deck_new();
    assert(deck != NULL);

    // Test that deck has exactly 52 cards
    assert(deck->size == 52);
    assert(deck->capacity == 52);

    deck_free(deck);

    printf("  ✓ Deck has correct size (52 cards)\n");
}

void test_deck_new_all_cards_unique(void) {
    printf("Testing all cards are unique...\n");

    Deck* deck = deck_new();
    assert(deck != NULL);

    // Check all 52 cards are unique
    // Compare each card with every other card
    for (size_t i = 0; i < deck->size; i++) {
        for (size_t j = i + 1; j < deck->size; j++) {
            // Cards are unique if rank OR suit differ
            int same_rank = (deck->cards[i].rank == deck->cards[j].rank);
            int same_suit = (deck->cards[i].suit == deck->cards[j].suit);

            // Both rank AND suit should not be identical
            assert(!(same_rank && same_suit));
        }
    }

    deck_free(deck);

    printf("  ✓ All 52 cards are unique (no duplicates)\n");
}

void test_deck_new_all_suits_represented(void) {
    printf("Testing all suits represented...\n");

    Deck* deck = deck_new();
    assert(deck != NULL);

    // Count cards of each suit
    int hearts_count = 0;
    int diamonds_count = 0;
    int clubs_count = 0;
    int spades_count = 0;

    for (size_t i = 0; i < deck->size; i++) {
        switch (deck->cards[i].suit) {
            case SUIT_HEARTS: hearts_count++; break;
            case SUIT_DIAMONDS: diamonds_count++; break;
            case SUIT_CLUBS: clubs_count++; break;
            case SUIT_SPADES: spades_count++; break;
        }
    }

    // Each suit should have exactly 13 cards
    assert(hearts_count == 13);
    assert(diamonds_count == 13);
    assert(clubs_count == 13);
    assert(spades_count == 13);

    deck_free(deck);

    printf("  ✓ All 4 suits represented (13 cards each)\n");
}

void test_deck_new_all_ranks_represented(void) {
    printf("Testing all ranks represented...\n");

    Deck* deck = deck_new();
    assert(deck != NULL);

    // Count cards of each rank
    int rank_counts[15] = {0}; // Index 2-14 for RANK_TWO to RANK_ACE

    for (size_t i = 0; i < deck->size; i++) {
        assert(deck->cards[i].rank >= RANK_TWO && deck->cards[i].rank <= RANK_ACE);
        rank_counts[deck->cards[i].rank]++;
    }

    // Each rank should appear exactly 4 times (once per suit)
    for (int rank = RANK_TWO; rank <= RANK_ACE; rank++) {
        assert(rank_counts[rank] == 4);
    }

    deck_free(deck);

    printf("  ✓ All 13 ranks represented (4 cards each)\n");
}

void test_deck_new_specific_cards_present(void) {
    printf("Testing specific cards present...\n");

    Deck* deck = deck_new();
    assert(deck != NULL);

    // Check for specific cards (Ace of Spades, Two of Hearts, King of Diamonds)
    int found_ace_spades = 0;
    int found_two_hearts = 0;
    int found_king_diamonds = 0;

    for (size_t i = 0; i < deck->size; i++) {
        if (deck->cards[i].rank == RANK_ACE && deck->cards[i].suit == SUIT_SPADES) {
            found_ace_spades = 1;
        }
        if (deck->cards[i].rank == RANK_TWO && deck->cards[i].suit == SUIT_HEARTS) {
            found_two_hearts = 1;
        }
        if (deck->cards[i].rank == RANK_KING && deck->cards[i].suit == SUIT_DIAMONDS) {
            found_king_diamonds = 1;
        }
    }

    assert(found_ace_spades);
    assert(found_two_hearts);
    assert(found_king_diamonds);

    deck_free(deck);

    printf("  ✓ Specific cards present in deck\n");
}

int main(void) {
    printf("\n=== Deck Test Suite ===\n\n");

    test_deck_new_allocation();
    test_deck_new_size();
    test_deck_new_all_cards_unique();
    test_deck_new_all_suits_represented();
    test_deck_new_all_ranks_represented();
    test_deck_new_specific_cards_present();

    printf("\n=== All deck tests passed! ===\n\n");
    return 0;
}
