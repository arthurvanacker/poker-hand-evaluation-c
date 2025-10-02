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

## Status

✅ **Phase 0 Complete** - Project structure, build system, and documentation framework established
✅ **Phase 1 Complete** - Foundation layer (Rank, Suit, Card, Deck) with full test coverage
🚧 Under Development - This README will be updated as each phase completes.

## License

MIT License - See [LICENSE](LICENSE)
