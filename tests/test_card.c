#include <assert.h>
#include <stdio.h>
#include <string.h>
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

void test_card_to_string_ranks(void) {
    printf("Testing card_to_string rank mapping...\n");
    char buffer[3];

    // Test rank 2-9 mapping
    Card card = {RANK_TWO, SUIT_HEARTS};
    assert(card_to_string(card, buffer, sizeof(buffer)) == 0);
    assert(strcmp(buffer, "2h") == 0);

    card.rank = RANK_THREE;
    assert(card_to_string(card, buffer, sizeof(buffer)) == 0);
    assert(strcmp(buffer, "3h") == 0);

    card.rank = RANK_FOUR;
    assert(card_to_string(card, buffer, sizeof(buffer)) == 0);
    assert(strcmp(buffer, "4h") == 0);

    card.rank = RANK_FIVE;
    assert(card_to_string(card, buffer, sizeof(buffer)) == 0);
    assert(strcmp(buffer, "5h") == 0);

    card.rank = RANK_SIX;
    assert(card_to_string(card, buffer, sizeof(buffer)) == 0);
    assert(strcmp(buffer, "6h") == 0);

    card.rank = RANK_SEVEN;
    assert(card_to_string(card, buffer, sizeof(buffer)) == 0);
    assert(strcmp(buffer, "7h") == 0);

    card.rank = RANK_EIGHT;
    assert(card_to_string(card, buffer, sizeof(buffer)) == 0);
    assert(strcmp(buffer, "8h") == 0);

    card.rank = RANK_NINE;
    assert(card_to_string(card, buffer, sizeof(buffer)) == 0);
    assert(strcmp(buffer, "9h") == 0);

    // Test Ten -> "T"
    card.rank = RANK_TEN;
    assert(card_to_string(card, buffer, sizeof(buffer)) == 0);
    assert(strcmp(buffer, "Th") == 0);

    // Test face cards
    card.rank = RANK_JACK;
    assert(card_to_string(card, buffer, sizeof(buffer)) == 0);
    assert(strcmp(buffer, "Jh") == 0);

    card.rank = RANK_QUEEN;
    assert(card_to_string(card, buffer, sizeof(buffer)) == 0);
    assert(strcmp(buffer, "Qh") == 0);

    card.rank = RANK_KING;
    assert(card_to_string(card, buffer, sizeof(buffer)) == 0);
    assert(strcmp(buffer, "Kh") == 0);

    card.rank = RANK_ACE;
    assert(card_to_string(card, buffer, sizeof(buffer)) == 0);
    assert(strcmp(buffer, "Ah") == 0);

    printf("  ✓ All rank mappings correct\n");
}

void test_card_to_string_suits(void) {
    printf("Testing card_to_string suit mapping...\n");
    char buffer[3];
    Card card = {RANK_ACE, SUIT_HEARTS};

    // Test all suit mappings
    card.suit = SUIT_HEARTS;
    assert(card_to_string(card, buffer, sizeof(buffer)) == 0);
    assert(strcmp(buffer, "Ah") == 0);

    card.suit = SUIT_DIAMONDS;
    assert(card_to_string(card, buffer, sizeof(buffer)) == 0);
    assert(strcmp(buffer, "Ad") == 0);

    card.suit = SUIT_CLUBS;
    assert(card_to_string(card, buffer, sizeof(buffer)) == 0);
    assert(strcmp(buffer, "Ac") == 0);

    card.suit = SUIT_SPADES;
    assert(card_to_string(card, buffer, sizeof(buffer)) == 0);
    assert(strcmp(buffer, "As") == 0);

    printf("  ✓ All suit mappings correct\n");
}

void test_card_to_string_all_52_cards(void) {
    printf("Testing card_to_string for all 52 cards...\n");

    char buffer[3];
    const char* rank_chars[] = {"2", "3", "4", "5", "6", "7", "8", "9", "T", "J", "Q", "K", "A"};
    const char* suit_chars[] = {"h", "d", "c", "s"};

    Rank ranks[] = {
        RANK_TWO, RANK_THREE, RANK_FOUR, RANK_FIVE, RANK_SIX,
        RANK_SEVEN, RANK_EIGHT, RANK_NINE, RANK_TEN, RANK_JACK,
        RANK_QUEEN, RANK_KING, RANK_ACE
    };

    Suit suits[] = {
        SUIT_HEARTS, SUIT_DIAMONDS, SUIT_CLUBS, SUIT_SPADES
    };

    // Test all 52 combinations
    for (int r = 0; r < 13; r++) {
        for (int s = 0; s < 4; s++) {
            Card card = {ranks[r], suits[s]};
            char expected[3];
            snprintf(expected, sizeof(expected), "%s%s", rank_chars[r], suit_chars[s]);

            assert(card_to_string(card, buffer, sizeof(buffer)) == 0);
            assert(strcmp(buffer, expected) == 0);
        }
    }

    printf("  ✓ All 52 card strings correct\n");
}

void test_card_to_string_buffer_overflow(void) {
    printf("Testing card_to_string buffer overflow protection...\n");

    Card card = {RANK_ACE, SUIT_SPADES};
    char buffer[3];

    // Test buffer too small (size < 3)
    assert(card_to_string(card, buffer, 0) == -1);
    assert(card_to_string(card, buffer, 1) == -1);
    assert(card_to_string(card, buffer, 2) == -1);

    // Test exact minimum size works
    assert(card_to_string(card, buffer, 3) == 0);
    assert(strcmp(buffer, "As") == 0);

    // Test larger buffer works
    char large_buffer[10];
    assert(card_to_string(card, large_buffer, sizeof(large_buffer)) == 0);
    assert(strcmp(large_buffer, "As") == 0);

    printf("  ✓ Buffer overflow protection works\n");
}

void test_card_to_string_edge_cases(void) {
    printf("Testing card_to_string edge cases...\n");

    char buffer[3];

    // Test lowest rank, lowest suit
    Card card = {RANK_TWO, SUIT_HEARTS};
    assert(card_to_string(card, buffer, sizeof(buffer)) == 0);
    assert(strcmp(buffer, "2h") == 0);

    // Test highest rank, highest suit
    card.rank = RANK_ACE;
    card.suit = SUIT_SPADES;
    assert(card_to_string(card, buffer, sizeof(buffer)) == 0);
    assert(strcmp(buffer, "As") == 0);

    printf("  ✓ Edge cases handled correctly\n");
}

void test_parse_card_valid_ranks(void) {
    printf("Testing parse_card with valid ranks...\n");
    Card card;

    // Test numeric ranks 2-9
    assert(parse_card("2h", &card) == 0);
    assert(card.rank == RANK_TWO);
    assert(card.suit == SUIT_HEARTS);

    assert(parse_card("3h", &card) == 0);
    assert(card.rank == RANK_THREE);

    assert(parse_card("4h", &card) == 0);
    assert(card.rank == RANK_FOUR);

    assert(parse_card("5h", &card) == 0);
    assert(card.rank == RANK_FIVE);

    assert(parse_card("6h", &card) == 0);
    assert(card.rank == RANK_SIX);

    assert(parse_card("7h", &card) == 0);
    assert(card.rank == RANK_SEVEN);

    assert(parse_card("8h", &card) == 0);
    assert(card.rank == RANK_EIGHT);

    assert(parse_card("9h", &card) == 0);
    assert(card.rank == RANK_NINE);

    // Test Ten (uppercase and lowercase)
    assert(parse_card("Th", &card) == 0);
    assert(card.rank == RANK_TEN);

    assert(parse_card("th", &card) == 0);
    assert(card.rank == RANK_TEN);

    // Test face cards (uppercase and lowercase)
    assert(parse_card("Jh", &card) == 0);
    assert(card.rank == RANK_JACK);

    assert(parse_card("jh", &card) == 0);
    assert(card.rank == RANK_JACK);

    assert(parse_card("Qh", &card) == 0);
    assert(card.rank == RANK_QUEEN);

    assert(parse_card("qh", &card) == 0);
    assert(card.rank == RANK_QUEEN);

    assert(parse_card("Kh", &card) == 0);
    assert(card.rank == RANK_KING);

    assert(parse_card("kh", &card) == 0);
    assert(card.rank == RANK_KING);

    assert(parse_card("Ah", &card) == 0);
    assert(card.rank == RANK_ACE);

    assert(parse_card("ah", &card) == 0);
    assert(card.rank == RANK_ACE);

    printf("  ✓ All valid ranks parsed correctly\n");
}

void test_parse_card_valid_suits(void) {
    printf("Testing parse_card with valid suits...\n");
    Card card;

    // Test hearts (uppercase and lowercase)
    assert(parse_card("Ah", &card) == 0);
    assert(card.suit == SUIT_HEARTS);

    assert(parse_card("AH", &card) == 0);
    assert(card.suit == SUIT_HEARTS);

    // Test diamonds (uppercase and lowercase)
    assert(parse_card("Ad", &card) == 0);
    assert(card.suit == SUIT_DIAMONDS);

    assert(parse_card("AD", &card) == 0);
    assert(card.suit == SUIT_DIAMONDS);

    // Test clubs (uppercase and lowercase)
    assert(parse_card("Ac", &card) == 0);
    assert(card.suit == SUIT_CLUBS);

    assert(parse_card("AC", &card) == 0);
    assert(card.suit == SUIT_CLUBS);

    // Test spades (uppercase and lowercase)
    assert(parse_card("As", &card) == 0);
    assert(card.suit == SUIT_SPADES);

    assert(parse_card("AS", &card) == 0);
    assert(card.suit == SUIT_SPADES);

    printf("  ✓ All valid suits parsed correctly\n");
}

void test_parse_card_null_pointer_errors(void) {
    printf("Testing parse_card NULL pointer handling...\n");
    Card card;

    // Test NULL string pointer
    assert(parse_card(NULL, &card) == -1);

    // Test NULL card pointer
    assert(parse_card("Ah", NULL) == -1);

    // Test both NULL
    assert(parse_card(NULL, NULL) == -1);

    printf("  ✓ NULL pointer errors handled correctly\n");
}

void test_parse_card_invalid_string_length(void) {
    printf("Testing parse_card invalid string length...\n");
    Card card;

    // Test empty string
    assert(parse_card("", &card) == -1);

    // Test too short (1 character)
    assert(parse_card("A", &card) == -1);

    // Test too long (3+ characters)
    assert(parse_card("Ahs", &card) == -1);
    assert(parse_card("AhXX", &card) == -1);

    printf("  ✓ Invalid string length rejected\n");
}

void test_parse_card_invalid_rank_characters(void) {
    printf("Testing parse_card invalid rank characters...\n");
    Card card;

    // Test invalid rank characters
    assert(parse_card("0h", &card) == -1);
    assert(parse_card("1h", &card) == -1);
    assert(parse_card("Xh", &card) == -1);
    assert(parse_card("Nh", &card) == -1);
    assert(parse_card("Bh", &card) == -1);

    printf("  ✓ Invalid rank characters rejected\n");
}

void test_parse_card_invalid_suit_characters(void) {
    printf("Testing parse_card invalid suit characters...\n");
    Card card;

    // Test invalid suit characters
    assert(parse_card("Ax", &card) == -1);
    assert(parse_card("Aa", &card) == -1);
    assert(parse_card("A1", &card) == -1);
    assert(parse_card("A0", &card) == -1);
    assert(parse_card("Az", &card) == -1);

    printf("  ✓ Invalid suit characters rejected\n");
}

void test_parse_card_bidirectional_conversion(void) {
    printf("Testing bidirectional parse ↔ to_string conversion...\n");

    // Test all 52 cards: parse → to_string → parse
    const char* test_cards[] = {
        "2h", "3h", "4h", "5h", "6h", "7h", "8h", "9h", "Th", "Jh", "Qh", "Kh", "Ah",
        "2d", "3d", "4d", "5d", "6d", "7d", "8d", "9d", "Td", "Jd", "Qd", "Kd", "Ad",
        "2c", "3c", "4c", "5c", "6c", "7c", "8c", "9c", "Tc", "Jc", "Qc", "Kc", "Ac",
        "2s", "3s", "4s", "5s", "6s", "7s", "8s", "9s", "Ts", "Js", "Qs", "Ks", "As"
    };

    for (int i = 0; i < 52; i++) {
        Card card1, card2;
        char buffer[3];

        // Parse original string
        assert(parse_card(test_cards[i], &card1) == 0);

        // Convert to string
        assert(card_to_string(card1, buffer, sizeof(buffer)) == 0);

        // Parse the converted string
        assert(parse_card(buffer, &card2) == 0);

        // Verify both cards are identical
        assert(card1.rank == card2.rank);
        assert(card1.suit == card2.suit);
    }

    printf("  ✓ Bidirectional conversion works for all 52 cards\n");
}

void test_parse_card_case_insensitive_combinations(void) {
    printf("Testing parse_card case-insensitive combinations...\n");
    Card card;

    // Test various case combinations
    assert(parse_card("ah", &card) == 0);
    assert(card.rank == RANK_ACE && card.suit == SUIT_HEARTS);

    assert(parse_card("AH", &card) == 0);
    assert(card.rank == RANK_ACE && card.suit == SUIT_HEARTS);

    assert(parse_card("Ah", &card) == 0);
    assert(card.rank == RANK_ACE && card.suit == SUIT_HEARTS);

    assert(parse_card("aH", &card) == 0);
    assert(card.rank == RANK_ACE && card.suit == SUIT_HEARTS);

    // Test with Ten
    assert(parse_card("td", &card) == 0);
    assert(card.rank == RANK_TEN && card.suit == SUIT_DIAMONDS);

    assert(parse_card("TD", &card) == 0);
    assert(card.rank == RANK_TEN && card.suit == SUIT_DIAMONDS);

    printf("  ✓ Case-insensitive combinations work correctly\n");
}

int main(void) {
    printf("\n=== Card Struct Test Suite ===\n\n");

    test_suit_values();
    test_suit_distinct();
    test_card_creation();
    test_card_size();
    test_all_52_cards();
    test_card_combinations();

    printf("\n=== Card To String Test Suite ===\n\n");
    test_card_to_string_ranks();
    test_card_to_string_suits();
    test_card_to_string_all_52_cards();
    test_card_to_string_buffer_overflow();
    test_card_to_string_edge_cases();

    printf("\n=== Parse Card Test Suite ===\n\n");
    test_parse_card_valid_ranks();
    test_parse_card_valid_suits();
    test_parse_card_null_pointer_errors();
    test_parse_card_invalid_string_length();
    test_parse_card_invalid_rank_characters();
    test_parse_card_invalid_suit_characters();
    test_parse_card_bidirectional_conversion();
    test_parse_card_case_insensitive_combinations();

    printf("\n=== All tests passed! ===\n\n");
    return 0;
}
