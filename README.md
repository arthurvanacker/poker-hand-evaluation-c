# Poker Hand Evaluator (C)

A Texas Hold'em poker hand evaluator written in C.

## Planned Features

- 5-card hand evaluation
- 6-card hand evaluation (best 5 from 6)
- 7-card hand evaluation (best 5 from 7, Texas Hold'em)

## Architecture

The project is built in phases:

- **Foundation**: Core data structures (Card, Deck, Rank, Suit)
- **Evaluation**: Hand representation and helper functions
- **Detection**: Hand category detectors (Royal Flush → High Card)
- **Integration**: Orchestration (`evaluate_hand()`, `compare_hands()`)
- **Polish**: Examples, benchmarks, documentation

## C-Specific Features

- Manual memory management with Valgrind verification
- Portable C99 with no external dependencies (except Unity tests)
- Performance-optimized algorithms
- Zero memory leaks verified across all test executables

## Documentation

Comprehensive documentation will be added as each phase completes:

- **Algorithm Explained** (`docs/ALGORITHM_EXPLAINED.md`) - Detailed explanation of hand evaluation algorithms (Phase 05)
- **Memory Safety** (`docs/MEMORY_SAFETY.md`) - Memory management practices and Valgrind verification (Phase 05)

Documentation structure is established early, with content added incrementally as features are implemented.

## Foundation Layer

The foundation layer provides core data structures for representing playing cards and decks.

### Rank and Suit Enums

**Rank** represents card values from 2 (deuce) to 14 (ace):
```c
typedef enum {
    RANK_TWO = 2,    RANK_THREE, RANK_FOUR,  RANK_FIVE,
    RANK_SIX,        RANK_SEVEN, RANK_EIGHT, RANK_NINE,
    RANK_TEN = 10,   RANK_JACK,  RANK_QUEEN, RANK_KING,
    RANK_ACE = 14
} Rank;
```

**Suit** represents the four standard suits:
```c
typedef enum {
    SUIT_HEARTS,
    SUIT_DIAMONDS,
    SUIT_CLUBS,
    SUIT_SPADES
} Suit;
```

### Card Structure

A `Card` is a compact 2-byte struct containing rank and suit:
```c
typedef struct {
    uint8_t rank;  /* 2-14 */
    uint8_t suit;  /* 0-3 */
} Card;
```

**Functions:**
- `int card_to_string(Card card, char* buffer, size_t size)` - Convert card to string (e.g., "Ah", "Td")
- `int parse_card(const char* str, Card* out_card)` - Parse string into Card struct

### Deck Structure

A `Deck` manages a dynamic array of cards:
```c
typedef struct {
    Card* cards;     /* Dynamic array */
    size_t size;     /* Current count */
    size_t capacity; /* Allocated capacity */
} Deck;
```

**Functions:**
- `Deck* deck_new(void)` - Create new 52-card deck (returns NULL on allocation failure)
- `void deck_shuffle(Deck* deck)` - Shuffle using Fisher-Yates algorithm (O(n) time, O(1) space)
- `size_t deck_deal(Deck* deck, Card* out_cards, size_t n)` - Deal n cards from deck
- `void deck_free(Deck* deck)` - Free all deck memory

### Usage Example

```c
#include "poker.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(void) {
    srand(time(NULL));
    Deck* deck = deck_new();
    deck_shuffle(deck);

    Card hand[5];
    deck_deal(deck, hand, 5);

    char card_str[3];
    for (int i = 0; i < 5; i++) {
        card_to_string(hand[i], card_str, sizeof(card_str));
        printf("%s ", card_str);
    }

    deck_free(deck);  /* Always free! */
    return 0;
}
```

**Memory Management:** Always call `deck_free()` to prevent memory leaks. The library uses manual memory management - Valgrind verification ensures zero leaks.

## Memory Safety and Valgrind

This project enforces zero memory leaks using Valgrind, the industry-standard memory debugging tool for C/C++ programs.

### Verifying Memory Safety

Run Valgrind on all test executables:

```bash
make valgrind
```

This command:
- Builds all test executables automatically
- Runs each test under Valgrind with full leak checking
- Reports any memory leaks, use-after-free, or invalid memory access
- Exits with non-zero status if any leaks are detected

**Expected output:**
```
==============================================
Valgrind Summary: 19/19 tests passed
✅ All tests passed - zero memory leaks!
```

### Manual Valgrind Testing

To test a specific executable:

```bash
# Build the test
gcc -Wall -Wextra -std=c99 -Iinclude tests/test_deck.c lib/libpoker.a -o build/test_deck

# Run under Valgrind
valgrind --leak-check=full \
         --show-leak-kinds=all \
         --track-origins=yes \
         --error-exitcode=1 \
         build/test_deck
```

### Valgrind Script

The `tests/valgrind_test.sh` script provides detailed Valgrind output for deck operations:

```bash
./tests/valgrind_test.sh
```

This script runs the deck test suite with verbose Valgrind output, useful for debugging memory issues.

### Memory Safety Guarantees

- **Zero leaks**: All dynamically allocated memory is properly freed
- **No use-after-free**: Freed memory is never accessed
- **No invalid reads/writes**: All memory accesses are within allocated bounds
- **NULL safety**: Functions handle NULL pointers gracefully

All 19 test executables pass Valgrind verification with zero errors.

## Evaluation Core

The evaluation core provides the data structures and helper functions needed to evaluate poker hands. This layer builds on the foundation to identify hand categories and compare hands.

### HandCategory Enum

The `HandCategory` enum represents all 10 poker hand types with explicit numeric values that enable direct comparison:

```c
typedef enum {
    HAND_HIGH_CARD = 1,
    HAND_ONE_PAIR = 2,
    HAND_TWO_PAIR = 3,
    HAND_THREE_OF_A_KIND = 4,
    HAND_STRAIGHT = 5,
    HAND_FLUSH = 6,
    HAND_FULL_HOUSE = 7,
    HAND_FOUR_OF_A_KIND = 8,
    HAND_STRAIGHT_FLUSH = 9,
    HAND_ROYAL_FLUSH = 10
} HandCategory;
```

The explicit discriminants allow numeric comparison: `HAND_ROYAL_FLUSH > HAND_FLUSH` evaluates to true.

### Hand Structure

The `Hand` struct represents an evaluated 5-card poker hand:

```c
#define MAX_TIEBREAKERS 5

typedef struct {
    Card cards[5];              /* Fixed-size array (exactly 5 cards) */
    HandCategory category;       /* Hand type */
    Rank tiebreakers[MAX_TIEBREAKERS]; /* Ranks for tiebreaking */
    size_t num_tiebreakers;     /* Number of valid tiebreakers */
} Hand;
```

**Fields:**
- `cards`: Fixed array containing exactly 5 cards
- `category`: The hand type (Royal Flush, Straight, High Card, etc.)
- `tiebreakers`: Ranks in descending order of importance for comparing hands of the same category
- `num_tiebreakers`: Number of valid tiebreaker values (0-5)

Using fixed-size arrays avoids dynamic allocation and simplifies memory management.

### Helper Functions

The evaluation core provides three key helper functions:

#### is_flush()

```c
int is_flush(const Card* cards, size_t len);
```

Checks if all cards are the same suit. Returns 1 if flush, 0 otherwise. Returns 0 if `len != 5`.

#### is_straight()

```c
int is_straight(const Card* cards, size_t len, Rank* out_high_card);
```

Checks if 5 cards form a sequence. Uses `qsort()` from `<stdlib.h>` to sort ranks in descending order, then validates the sequence.

**Wheel Straight Special Case:** The ace-low straight A-2-3-4-5 (the "wheel") is treated specially. When detected, the function returns 1 and sets `out_high_card` to `RANK_FIVE` (not `RANK_ACE`), since the five is the high card in this straight.

**Parameters:**
- `cards`: Array of cards to check
- `len`: Number of cards (must be 5)
- `out_high_card`: Pointer to receive the high card rank (can be NULL)

**Returns:** 1 if straight, 0 otherwise

#### rank_counts()

```c
void rank_counts(const Card* cards, size_t len, int* counts);
```

Counts the frequency of each rank using an array-based approach (not a HashMap). The caller must provide an array of at least 15 integers, indexed by rank value (0-14).

**Example:** For four aces and a king:
- `counts[RANK_ACE]` = 4
- `counts[RANK_KING]` = 1
- All other indices = 0

This function is used to detect pairs, three-of-a-kind, four-of-a-kind, and full house patterns.

### Evaluation Architecture

The evaluation system follows a layered design:

1. **Foundation Layer** (Phase 01): Basic types (Rank, Suit, Card, Deck)
2. **Evaluation Core** (Phase 02): Hand representation and helper functions
3. **Detection Layer** (Phase 03): Hand category detectors (Royal Flush → High Card)
4. **Integration Layer** (Phase 04): Orchestration (`evaluate_hand()`, `compare_hands()`)

The helper functions use C standard library features:
- `qsort()` for sorting ranks
- Array-based counting (not HashMap) for rank frequency analysis
- Fixed-size arrays to avoid dynamic allocation

## Detection Layer

The detection layer provides 10 specialized functions to identify specific poker hand categories. Each detector returns 1 if the hand matches the category, 0 otherwise. Most detectors also populate output parameters with tiebreaker ranks used for comparing hands of the same category.

### Return Codes

All detection functions follow a consistent return convention:
- **1** = Hand matches this category
- **0** = Hand does not match this category

### Output Parameters

Detection functions use C-style output parameters to return tiebreaker information:

- **`out_tiebreakers`**: Array to receive tiebreaker ranks (caller-allocated, must hold at least `MAX_TIEBREAKERS` elements)
- **`out_num_tiebreakers`**: Pointer to receive the count of valid tiebreaker ranks
- **`out_high_card`**: Pointer to receive the high card rank (used by straight-based detectors)

Tiebreaker ranks are stored in descending order of importance for hand comparison.

### Detection Functions

#### detect_royal_flush()

```c
int detect_royal_flush(const Card* cards, size_t len);
```

Detects royal flush (A-K-Q-J-T of the same suit). Returns 1 if detected, 0 otherwise.

**Tiebreakers:** None (royal flush has no tiebreakers - all royal flushes tie)

**Example:**
- `Ah Kh Qh Jh Th` → Returns 1

#### detect_straight_flush()

```c
int detect_straight_flush(const Card* cards, size_t len, Rank* out_high_card);
```

Detects straight flush (5 consecutive ranks, same suit, not royal flush). Returns 1 if detected, 0 otherwise.

**Tiebreakers:** `[high_card]`
- `high_card`: The highest rank in the straight

**Wheel Straight Special Case:** For A-2-3-4-5 straight flush (wheel), the high card is `RANK_FIVE` (not `RANK_ACE`), following standard poker rules.

**Example:**
- `9s 8s 7s 6s 5s` → Returns 1, `out_high_card = RANK_NINE`
- `5d 4d 3d 2d Ad` → Returns 1, `out_high_card = RANK_FIVE` (wheel)

#### detect_four_of_a_kind()

```c
int detect_four_of_a_kind(const Card* cards, size_t len,
                           const int* counts,
                           Rank* out_tiebreakers,
                           size_t* out_num_tiebreakers);
```

Detects four of a kind (4 cards of the same rank). Returns 1 if detected, 0 otherwise.

**Tiebreakers:** `[quad_rank, kicker]`
- `quad_rank`: The rank of the four-of-a-kind
- `kicker`: The rank of the remaining card

**Example:**
- `Ks Kh Kd Kc 7h` → Returns 1, tiebreakers = `[RANK_KING, RANK_SEVEN]`

#### detect_full_house()

```c
int detect_full_house(const Card* cards, size_t len,
                      const int* counts,
                      Rank* out_tiebreakers,
                      size_t* out_num_tiebreakers);
```

Detects full house (3 cards of one rank, 2 cards of another rank). Returns 1 if detected, 0 otherwise.

**Tiebreakers:** `[trip_rank, pair_rank]`
- `trip_rank`: The rank of the three-of-a-kind
- `pair_rank`: The rank of the pair

**Example:**
- `Jd Jc Jh 8s 8d` → Returns 1, tiebreakers = `[RANK_JACK, RANK_EIGHT]`

#### detect_flush()

```c
int detect_flush(const Card* cards, size_t len,
                 Rank* out_tiebreakers,
                 size_t* out_num_tiebreakers);
```

Detects flush (5 cards of the same suit, not a straight). Returns 1 if detected, 0 otherwise.

**Tiebreakers:** `[r1, r2, r3, r4, r5]`
- All 5 card ranks in descending order

**Example:**
- `Kh Jh 9h 6h 2h` → Returns 1, tiebreakers = `[RANK_KING, RANK_JACK, RANK_NINE, RANK_SIX, RANK_TWO]`

**Note:** Uses `qsort()` to sort ranks in descending order before storing tiebreakers.

#### detect_straight()

```c
int detect_straight(const Card* cards, size_t len,
                    Rank* out_tiebreakers,
                    size_t* out_num_tiebreakers);
```

Detects straight (5 consecutive ranks, not all the same suit). Returns 1 if detected, 0 otherwise.

**Tiebreakers:** `[high_card]`
- `high_card`: The highest rank in the straight

**Wheel Straight Special Case:** For A-2-3-4-5 straight (wheel), the high card is `RANK_FIVE` (not `RANK_ACE`).

**Example:**
- `9c 8h 7d 6s 5c` → Returns 1, tiebreakers = `[RANK_NINE]`
- `5h 4d 3c 2s Ah` → Returns 1, tiebreakers = `[RANK_FIVE]` (wheel)

#### detect_three_of_a_kind()

```c
int detect_three_of_a_kind(const Card* cards, size_t len,
                            const int* counts,
                            Rank* out_tiebreakers,
                            size_t* out_num_tiebreakers);
```

Detects three of a kind (3 cards of the same rank, no pair). Returns 1 if detected, 0 otherwise.

**Tiebreakers:** `[trip_rank, k1, k2]`
- `trip_rank`: The rank of the three-of-a-kind
- `k1`, `k2`: The two kicker ranks in descending order

**Example:**
- `Qh Qd Qc Ts 7h` → Returns 1, tiebreakers = `[RANK_QUEEN, RANK_TEN, RANK_SEVEN]`

#### detect_two_pair()

```c
int detect_two_pair(const Card* cards, size_t len,
                    const int* counts,
                    Rank* out_tiebreakers,
                    size_t* out_num_tiebreakers);
```

Detects two pair (2 cards of one rank, 2 cards of another rank). Returns 1 if detected, 0 otherwise.

**Tiebreakers:** `[high_pair, low_pair, kicker]`
- `high_pair`: The rank of the higher pair
- `low_pair`: The rank of the lower pair
- `kicker`: The rank of the remaining card

**Example:**
- `Ah Ad 9c 9s 5h` → Returns 1, tiebreakers = `[RANK_ACE, RANK_NINE, RANK_FIVE]`

#### detect_one_pair()

```c
int detect_one_pair(const Card* cards, size_t len,
                    const int* counts,
                    Rank* out_tiebreakers,
                    size_t* out_num_tiebreakers);
```

Detects one pair (2 cards of the same rank, no other pairs). Returns 1 if detected, 0 otherwise.

**Tiebreakers:** `[pair_rank, k1, k2, k3]`
- `pair_rank`: The rank of the pair
- `k1`, `k2`, `k3`: The three kicker ranks in descending order

**Example:**
- `Tc Th 8d 6s 3c` → Returns 1, tiebreakers = `[RANK_TEN, RANK_EIGHT, RANK_SIX, RANK_THREE]`

#### detect_high_card()

```c
int detect_high_card(const Card* cards, size_t len,
                     Rank* out_tiebreakers,
                     size_t* out_num_tiebreakers);
```

Detects high card (no other hand category). Always returns 1 for valid 5-card input.

**Tiebreakers:** `[r1, r2, r3, r4, r5]`
- All 5 card ranks in descending order

**Example:**
- `Kd Jc 9h 7s 3d` → Returns 1, tiebreakers = `[RANK_KING, RANK_JACK, RANK_NINE, RANK_SEVEN, RANK_THREE]`

**Note:** Uses `qsort()` to sort ranks in descending order before storing tiebreakers.

### Implementation Notes

**Sorting with qsort():**
The detection functions use `qsort()` from `<stdlib.h>` to sort ranks in descending order when needed (flush, high card, and internally for straight detection).

**Wheel Straight Special Case:**
The A-2-3-4-5 straight (wheel) is treated specially in poker. The ace acts as a low card, making the five the high card. Both `detect_straight()` and `detect_straight_flush()` correctly handle this edge case by returning `RANK_FIVE` as the high card.

**Optional Pre-computed Counts:**
Functions that analyze rank patterns (four of a kind, full house, three of a kind, two pair, one pair) accept an optional `counts` parameter. If `NULL`, the function computes counts internally using `rank_counts()`. If provided, the function uses the pre-computed counts for efficiency.

## Status

✅ **Phase 0 Complete** - Project structure, build system, and documentation framework established
✅ **Phase 1 Complete** - Foundation layer (Rank, Suit, Card, Deck) with full test coverage
✅ **Phase 2 Complete** - Evaluation core (HandCategory, Hand, helper functions) with comprehensive tests
✅ **Phase 3 Complete** - Detection layer (10 hand category detectors from Royal Flush to High Card)
🚧 Under Development - This README will be updated as each phase completes.

## License

MIT License - See [LICENSE](LICENSE)
