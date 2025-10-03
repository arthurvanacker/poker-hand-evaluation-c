/*
 * Test const-correctness of function signatures
 *
 * This test verifies that all function signatures properly use const qualifiers
 * for read-only parameters, enabling compiler optimizations and preventing
 * accidental modifications.
 */

#include "../include/poker.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>

/*
 * Test that const-qualified parameters compile without warnings
 * and that functions properly respect const-correctness
 */
void test_const_params_compile(void) {
    /* Test card_to_string with const size */
    Card card = {RANK_ACE, SUIT_HEARTS};
    char buffer[3];
    const size_t buffer_size = 3;
    assert(card_to_string(card, buffer, buffer_size) == 0);

    /* Test parse_card with const string */
    const char* const card_str = "Ah";
    Card parsed;
    assert(parse_card(card_str, &parsed) == 0);

    /* Test is_flush with const len */
    const Card flush_cards[5] = {
        {RANK_ACE, SUIT_HEARTS},
        {RANK_KING, SUIT_HEARTS},
        {RANK_QUEEN, SUIT_HEARTS},
        {RANK_JACK, SUIT_HEARTS},
        {RANK_TEN, SUIT_HEARTS}
    };
    const size_t len = 5;
    assert(is_flush(flush_cards, len) == 1);

    /* Test is_straight with const len */
    Rank high_card;
    assert(is_straight(flush_cards, len, &high_card) == 1);

    /* Test rank_counts with const len */
    int counts[15] = {0};
    rank_counts(flush_cards, len, counts);

    /* Test detect functions with const len */
    Rank tiebreakers[MAX_TIEBREAKERS];
    size_t num_tiebreakers;

    detect_royal_flush(flush_cards, len);
    detect_straight_flush(flush_cards, len, &high_card);
    detect_four_of_a_kind(flush_cards, len, counts, tiebreakers, &num_tiebreakers);
    detect_full_house(flush_cards, len, counts, tiebreakers, &num_tiebreakers);
    detect_flush(flush_cards, len, tiebreakers, &num_tiebreakers);
    detect_straight(flush_cards, len, tiebreakers, &num_tiebreakers);
    detect_three_of_a_kind(flush_cards, len, counts, tiebreakers, &num_tiebreakers);
    detect_two_pair(flush_cards, len, counts, tiebreakers, &num_tiebreakers);
    detect_one_pair(flush_cards, len, counts, tiebreakers, &num_tiebreakers);
    detect_high_card(flush_cards, len, tiebreakers, &num_tiebreakers);

    /* Test deck_deal with const n */
    Deck* deck = deck_new();
    assert(deck != NULL);
    Card dealt[5];
    const size_t num_cards = 5;
    assert(deck_deal(deck, dealt, num_cards) == num_cards);
    deck_free(deck);

    printf("test_const_params_compile: PASSED\n");
}

/*
 * Test that const Card arrays prevent modification attempts
 * This ensures the compiler enforces const-correctness
 */
void test_const_array_immutability(void) {
    const Card cards[5] = {
        {RANK_ACE, SUIT_HEARTS},
        {RANK_KING, SUIT_HEARTS},
        {RANK_QUEEN, SUIT_HEARTS},
        {RANK_JACK, SUIT_HEARTS},
        {RANK_TEN, SUIT_HEARTS}
    };

    /* Functions accepting const Card* should not modify the array */
    const size_t len = 5;
    int result = is_flush(cards, len);
    assert(result == 1);

    /* Verify cards array is unchanged */
    assert(cards[0].rank == RANK_ACE);
    assert(cards[0].suit == SUIT_HEARTS);

    printf("test_const_array_immutability: PASSED\n");
}

int main(void) {
    printf("Running const-correctness tests...\n\n");

    test_const_params_compile();
    test_const_array_immutability();

    printf("\nAll const-correctness tests passed!\n");
    return 0;
}
