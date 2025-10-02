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

void test_deck_shuffle_preserves_all_cards(void) {
    printf("Testing deck_shuffle preserves all 52 unique cards...\n");

    Deck* deck = deck_new();
    assert(deck != NULL);

    // Shuffle the deck
    deck_shuffle(deck);

    // Verify still 52 cards
    assert(deck->size == 52);

    // Verify all cards are still unique
    for (size_t i = 0; i < deck->size; i++) {
        for (size_t j = i + 1; j < deck->size; j++) {
            int same_rank = (deck->cards[i].rank == deck->cards[j].rank);
            int same_suit = (deck->cards[i].suit == deck->cards[j].suit);
            assert(!(same_rank && same_suit));
        }
    }

    // Verify all 13 ranks present (4 of each)
    int rank_counts[15] = {0};
    for (size_t i = 0; i < deck->size; i++) {
        rank_counts[deck->cards[i].rank]++;
    }
    for (int rank = RANK_TWO; rank <= RANK_ACE; rank++) {
        assert(rank_counts[rank] == 4);
    }

    // Verify all 4 suits present (13 of each)
    int suit_counts[4] = {0};
    for (size_t i = 0; i < deck->size; i++) {
        suit_counts[deck->cards[i].suit]++;
    }
    for (int suit = SUIT_HEARTS; suit <= SUIT_SPADES; suit++) {
        assert(suit_counts[suit] == 13);
    }

    deck_free(deck);

    printf("  ✓ Shuffled deck preserves all 52 unique cards\n");
}

void test_deck_shuffle_changes_order(void) {
    printf("Testing deck_shuffle changes card order...\n");

    Deck* deck = deck_new();
    assert(deck != NULL);

    // Save original order
    Card original[52];
    for (size_t i = 0; i < 52; i++) {
        original[i] = deck->cards[i];
    }

    // Seed random for reproducible test
    srand(42);
    deck_shuffle(deck);

    // Check that order has changed
    // At least one card should be in a different position
    int order_changed = 0;
    for (size_t i = 0; i < 52; i++) {
        if (deck->cards[i].rank != original[i].rank ||
            deck->cards[i].suit != original[i].suit) {
            order_changed = 1;
            break;
        }
    }
    assert(order_changed);

    deck_free(deck);

    printf("  ✓ Shuffled deck has different order than original\n");
}

void test_deck_shuffle_randomness(void) {
    printf("Testing deck_shuffle produces different results with different seeds...\n");

    Deck* deck1 = deck_new();
    Deck* deck2 = deck_new();
    assert(deck1 != NULL && deck2 != NULL);

    // Shuffle with different seeds
    srand(12345);
    deck_shuffle(deck1);

    srand(67890);
    deck_shuffle(deck2);

    // Check that the two shuffles produced different results
    int different = 0;
    for (size_t i = 0; i < 52; i++) {
        if (deck1->cards[i].rank != deck2->cards[i].rank ||
            deck1->cards[i].suit != deck2->cards[i].suit) {
            different = 1;
            break;
        }
    }
    assert(different);

    deck_free(deck1);
    deck_free(deck2);

    printf("  ✓ Different seeds produce different shuffle results\n");
}

void test_deck_deal_basic(void) {
    printf("Testing deck_deal basic functionality...\n");

    Deck* deck = deck_new();
    assert(deck != NULL);
    assert(deck->size == 52);

    // Deal 5 cards
    Card dealt_cards[5];
    size_t dealt_count = deck_deal(deck, dealt_cards, 5);

    // Verify return value is correct
    assert(dealt_count == 5);

    // Verify deck size decreased
    assert(deck->size == 47);

    // Verify dealt cards match first 5 cards from original deck
    Deck* original_deck = deck_new();
    for (size_t i = 0; i < 5; i++) {
        assert(dealt_cards[i].rank == original_deck->cards[i].rank);
        assert(dealt_cards[i].suit == original_deck->cards[i].suit);
    }

    deck_free(deck);
    deck_free(original_deck);

    printf("  ✓ Deal 5 cards reduces deck size and copies cards correctly\n");
}

void test_deck_deal_zero_cards(void) {
    printf("Testing deck_deal with zero cards...\n");

    Deck* deck = deck_new();
    assert(deck != NULL);
    size_t original_size = deck->size;

    // Deal 0 cards
    Card dealt_cards[1];
    size_t dealt_count = deck_deal(deck, dealt_cards, 0);

    // Verify return value is 0
    assert(dealt_count == 0);

    // Verify deck size unchanged
    assert(deck->size == original_size);

    deck_free(deck);

    printf("  ✓ Deal 0 cards leaves deck unchanged\n");
}

void test_deck_deal_all_cards(void) {
    printf("Testing deck_deal with all 52 cards...\n");

    Deck* deck = deck_new();
    assert(deck != NULL);

    // Deal all 52 cards
    Card dealt_cards[52];
    size_t dealt_count = deck_deal(deck, dealt_cards, 52);

    // Verify return value is 52
    assert(dealt_count == 52);

    // Verify deck is now empty
    assert(deck->size == 0);

    // Verify all cards were dealt
    Deck* original_deck = deck_new();
    for (size_t i = 0; i < 52; i++) {
        assert(dealt_cards[i].rank == original_deck->cards[i].rank);
        assert(dealt_cards[i].suit == original_deck->cards[i].suit);
    }

    deck_free(deck);
    deck_free(original_deck);

    printf("  ✓ Deal all 52 cards empties deck completely\n");
}

void test_deck_deal_more_than_available(void) {
    printf("Testing deck_deal with more cards than available...\n");

    Deck* deck = deck_new();
    assert(deck != NULL);
    assert(deck->size == 52);

    // Try to deal 53 cards (more than available)
    Card dealt_cards[53];
    size_t dealt_count = deck_deal(deck, dealt_cards, 53);

    // Verify return value is 52 (actual number dealt)
    assert(dealt_count == 52);

    // Verify deck is now empty
    assert(deck->size == 0);

    // Verify only 52 cards were dealt
    Deck* original_deck = deck_new();
    for (size_t i = 0; i < 52; i++) {
        assert(dealt_cards[i].rank == original_deck->cards[i].rank);
        assert(dealt_cards[i].suit == original_deck->cards[i].suit);
    }

    deck_free(deck);
    deck_free(original_deck);

    printf("  ✓ Deal 53 cards returns only 52 available cards\n");
}

void test_deck_deal_multiple_times(void) {
    printf("Testing deck_deal called multiple times...\n");

    Deck* deck = deck_new();
    assert(deck != NULL);
    assert(deck->size == 52);

    // Deal 5 cards
    Card first_deal[5];
    size_t first_count = deck_deal(deck, first_deal, 5);
    assert(first_count == 5);
    assert(deck->size == 47);

    // Deal another 10 cards
    Card second_deal[10];
    size_t second_count = deck_deal(deck, second_deal, 10);
    assert(second_count == 10);
    assert(deck->size == 37);

    // Deal remaining 37 cards
    Card third_deal[37];
    size_t third_count = deck_deal(deck, third_deal, 37);
    assert(third_count == 37);
    assert(deck->size == 0);

    // Verify cards are dealt sequentially
    Deck* original_deck = deck_new();
    for (size_t i = 0; i < 5; i++) {
        assert(first_deal[i].rank == original_deck->cards[i].rank);
        assert(first_deal[i].suit == original_deck->cards[i].suit);
    }
    for (size_t i = 0; i < 10; i++) {
        assert(second_deal[i].rank == original_deck->cards[5 + i].rank);
        assert(second_deal[i].suit == original_deck->cards[5 + i].suit);
    }
    for (size_t i = 0; i < 37; i++) {
        assert(third_deal[i].rank == original_deck->cards[15 + i].rank);
        assert(third_deal[i].suit == original_deck->cards[15 + i].suit);
    }

    deck_free(deck);
    deck_free(original_deck);

    printf("  ✓ Multiple deals work sequentially\n");
}

void test_deck_deal_from_empty_deck(void) {
    printf("Testing deck_deal from empty deck...\n");

    Deck* deck = deck_new();
    assert(deck != NULL);

    // Empty the deck first
    Card discard[52];
    deck_deal(deck, discard, 52);
    assert(deck->size == 0);

    // Try to deal from empty deck
    Card dealt_cards[5];
    size_t dealt_count = deck_deal(deck, dealt_cards, 5);

    // Verify return value is 0
    assert(dealt_count == 0);

    // Verify deck is still empty
    assert(deck->size == 0);

    deck_free(deck);

    printf("  ✓ Deal from empty deck returns 0 cards\n");
}

int main(void) {
    printf("\n=== Deck Test Suite ===\n\n");

    test_deck_new_allocation();
    test_deck_new_size();
    test_deck_new_all_cards_unique();
    test_deck_new_all_suits_represented();
    test_deck_new_all_ranks_represented();
    test_deck_new_specific_cards_present();
    test_deck_shuffle_preserves_all_cards();
    test_deck_shuffle_changes_order();
    test_deck_shuffle_randomness();
    test_deck_deal_basic();
    test_deck_deal_zero_cards();
    test_deck_deal_all_cards();
    test_deck_deal_more_than_available();
    test_deck_deal_multiple_times();
    test_deck_deal_from_empty_deck();

    printf("\n=== All deck tests passed! ===\n\n");
    return 0;
}
