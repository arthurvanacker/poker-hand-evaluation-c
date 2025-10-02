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

## Status

✅ **Phase 0 Complete** - Project structure, build system, and documentation framework established
✅ **Phase 1 Complete** - Foundation layer (Rank, Suit, Card, Deck) with full test coverage
✅ **Phase 2 Complete** - Evaluation core (HandCategory, Hand, helper functions) with comprehensive tests
🚧 Under Development - This README will be updated as each phase completes.

## License

MIT License - See [LICENSE](LICENSE)
