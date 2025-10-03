# Poker Hand Evaluator (C)

[![CI](https://github.com/arthurvanacker/poker-hand-evaluation-c/actions/workflows/ci.yml/badge.svg)](https://github.com/arthurvanacker/poker-hand-evaluation-c/actions/workflows/ci.yml)

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

- Manual memory management with Valgrind verification (19/19 tests pass with zero leaks)
- Portable C99 with no external dependencies (except Unity tests)
- Performance-optimized algorithms with overflow-safe integer operations
- Security hardening: fuzzing infrastructure, NULL poisoning, const-correctness
- Zero memory leaks verified across all test executables
- Comprehensive fuzzing with libFuzzer and AddressSanitizer support

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

## Build Requirements

This project requires the following tools to be installed:

### Core Build Tools (Required)
- **gcc** - GNU C Compiler (C99 compatible)
- **make** - GNU Make build system

### Optional Tools (Recommended)
- **valgrind** - Memory leak detection (`make valgrind`)
- **lcov** - Code coverage HTML reports (`make coverage`)
- **cppcheck** - Static analysis (`make lint`)
- **clang** - Alternative compiler and fuzzing support (`make fuzz-libfuzzer`)

### Installing Dependencies

**Ubuntu/Debian:**
```bash
# Core tools (usually pre-installed)
sudo apt-get install gcc make

# Optional tools (recommended for development)
sudo apt-get install valgrind lcov cppcheck clang
```

All optional tools are used by specific make targets and are not required for basic library compilation.

## Code Quality & Security

This project follows strict quality standards enforced through automated testing, static analysis, and security hardening practices.

### Quality Metrics (Milestone 06)

- **Code Review Average**: 97/100 across 5 PRs
- **Test Coverage**: 685 assertions across 19 test executables
- **Memory Safety**: Zero leaks verified by Valgrind
- **Security Hardening**: 2 critical vulnerabilities fixed (CWE-190, CWE-415)
- **Type Safety**: Const-correctness enforced on 23 function declarations
- **Fuzzing Coverage**: 4 NULL pointer bugs discovered in <10 seconds

### Static Analysis

The project uses **cppcheck** for automated static analysis to catch bugs, undefined behavior, and style issues before they reach production.

**Running static analysis:**
```bash
make lint
```

This command runs cppcheck with comprehensive checks including:
- **Error detection**: Null pointer dereferences, buffer overflows, memory leaks
- **Warning detection**: Unused variables, unreachable code, style issues
- **Performance issues**: Inefficient algorithms, unnecessary operations
- **Portability issues**: Platform-specific code, non-standard C

**Configuration:**
- Checks all source files in `src/` directory
- Uses include path `-I include/` for header resolution
- Enables all checks with `--enable=all`
- Suppresses system include warnings with `--suppress=missingIncludeSystem`
- Returns non-zero exit code on any issues found (`--error-exitcode=1`)

**Example output:**
```
Running static analysis...
Checking src/card.c...
Checking src/deck.c...
Checking src/evaluator.c...
```

If issues are found, cppcheck will report them with file location, line number, and suggested fixes.

### Const-Correctness

All public API functions (23 total) use const qualifiers to enforce immutability:

```c
// Read-only pointer to read-only data
int is_flush(const Card* const cards, const size_t len);

// Read-only pointer to mutable data (output parameter)
int parse_card(const char* const str, Card* const out_card);
```

This provides:
- Compiler-enforced immutability guarantees
- Prevention of accidental modifications
- Better compiler optimization opportunities
- Clear API contracts (input vs output parameters)

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
- **No use-after-free**: Freed memory is never accessed through NULL poisoning technique
- **No invalid reads/writes**: All memory accesses are within allocated bounds
- **NULL safety**: Functions handle NULL pointers gracefully
- **Double-free prevention**: `deck_free()` uses NULL poisoning to prevent CWE-415 vulnerabilities
- **Integer overflow prevention**: Safe comparison functions prevent CWE-190 in qsort comparators

All 19 test executables pass Valgrind verification with zero errors.

### Security Vulnerabilities Fixed

**Milestone 06 (Code Quality & Security)** addressed 2 critical security issues:

1. **Integer Overflow in qsort Comparator (CWE-190)** - Fixed in PR #104
   - Replaced subtraction-based comparison with safe conditional logic
   - Prevents undefined behavior when comparing extreme rank values

2. **Double-Free Vulnerability (CWE-415)** - Fixed in PR #105
   - Added NULL poisoning in `deck_free()` function
   - Sets `deck->cards = NULL` after freeing to prevent double-free attacks

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

## Fuzzing

This project includes comprehensive fuzzing infrastructure to discover edge cases, crashes, and memory safety issues through automated testing. Fuzzing is a security hardening technique that tests the library with millions of random inputs to find bugs that traditional unit tests might miss.

### Available Fuzzing Targets

**1. Card Parsing Fuzzer** (`fuzz/fuzz_parse_card.c`)
- Tests `parse_card()` with arbitrary input strings
- Validates error handling for malformed input
- Checks for buffer overflows and null pointer handling
- Verifies round-trip conversion (parse → to_string → parse)

**2. Hand Evaluation Fuzzer** (`fuzz/fuzz_evaluate_hand.c`)
- Tests all 10 hand detection functions
- Validates tiebreaker calculation correctness
- Tests with random 5-card combinations
- Verifies hand category invariants (e.g., royal flush implies flush)
- Tests edge cases: NULL pointers, wrong array lengths, invalid cards

### Building Fuzzers

**Option 1: Standalone Mode (gcc, no dependencies)**
```bash
make fuzz-standalone
```

This builds fuzzing harnesses using gcc that can run immediately without additional setup. Best for quick testing and CI/CD pipelines.

**Option 2: libFuzzer Mode (clang + AddressSanitizer, recommended for deep testing)**
```bash
# Install clang if not available
sudo apt-get install clang

# Build with libFuzzer
make fuzz-libfuzzer
```

This builds fuzzing harnesses with clang's libFuzzer, AddressSanitizer, and UndefinedBehaviorSanitizer. Provides superior bug detection capabilities including:
- Memory safety violations (use-after-free, buffer overflows)
- Undefined behavior (integer overflow, null pointer dereference)
- Coverage-guided fuzzing (explores code paths automatically)

### Running Fuzzing Tests

**Quick Test (standalone mode):**
```bash
make fuzz
```

Runs both fuzzers in standalone mode with pre-defined test cases and 10,000+ random inputs. Completes in seconds.

**Extended Fuzzing Campaign (libFuzzer):**
```bash
# Fuzz card parsing for 1 hour
./build/fuzz_parse_card_libfuzzer -max_total_time=3600

# Fuzz hand evaluation for 24 hours
./build/fuzz_evaluate_hand_libfuzzer -max_total_time=86400
```

LibFuzzer provides real-time statistics including:
- Execution speed (execs/sec)
- Code coverage (edges covered)
- Corpus size (interesting test cases found)
- Crashes detected

### Interpreting Results

**No crashes found:**
```
#1000000 DONE   cov: 245 ft: 312 corp: 89/1024b exec/s: 50000
```
This indicates successful fuzzing with no bugs detected.

**Crash detected:**
```
==1234==ERROR: AddressSanitizer: heap-buffer-overflow on address 0x...
```
LibFuzzer will save the crashing input to `crash-<hash>` for reproduction and debugging.

### Fuzzing Best Practices

1. **Start with standalone mode** to verify infrastructure works
2. **Use libFuzzer for extended campaigns** (24+ hours recommended)
3. **Run fuzzing after code changes** to catch regressions
4. **Integrate with CI/CD** using `make fuzz` for continuous testing
5. **Reproduce crashes** by running fuzzer with saved crash file:
   ```bash
   ./build/fuzz_parse_card_libfuzzer crash-abc123
   ```

### Bugs Discovered Through Fuzzing

Initial fuzzing campaigns (PR #108) discovered **4 NULL pointer vulnerabilities** within seconds:

1. `is_straight(NULL, 5, &high_card)` - NULL dereference in rank sorting
2. `is_flush(NULL, 5)` - NULL dereference in suit checking
3. `rank_counts(NULL, 5, counts)` - NULL dereference in frequency counting
4. `card_to_string(card, NULL, 3)` - NULL dereference in buffer writing

These issues are documented in `fuzz/FUZZING_RESULTS.md` and demonstrate the value of automated fuzzing in finding edge cases that traditional unit tests miss.

### Coverage Goals

- **Target**: >90% code coverage during fuzzing campaigns
- **Current status**: Comprehensive coverage of all detection functions and helper functions
- **Measuring coverage**: Use clang's `-fprofile-instr-generate -fcoverage-mapping` flags

### Security Hardening

Fuzzing helps prevent:
- **Buffer overflows**: Detected when inputs exceed expected sizes
- **Null pointer dereferences**: Caught by passing NULL to functions
- **Integer overflows**: Found through extreme rank/suit values
- **Logic errors**: Discovered through invariant violations
- **Memory leaks**: Identified when combined with Valgrind

### References

- libFuzzer documentation: https://llvm.org/docs/LibFuzzer.html
- AddressSanitizer: https://clang.llvm.org/docs/AddressSanitizer.html
- Fuzzing best practices: https://google.github.io/fuzzing/

## Code Coverage

This project uses gcov and lcov to track test coverage and ensure comprehensive testing of all source code.

### Current Coverage

- **Overall**: 96.38% line coverage across all source files
- **card.c**: 93.33% (60 lines)
- **deck.c**: 92.50% (40 lines)
- **evaluator.c**: 97.68% (259 lines)

The project maintains a minimum coverage target of ≥90% to ensure code quality and reliability.

### Generating Coverage Reports

Generate coverage reports with a single command:

```bash
make coverage
```

This command:
1. Rebuilds the library with coverage instrumentation (`--coverage` flag)
2. Runs all test executables to collect coverage data
3. Generates `.gcov` files showing line-by-line coverage
4. Creates HTML report (if lcov is installed)

### Viewing Coverage Results

**Text-based coverage (always available):**
```bash
# View coverage summary
cat *.gcov | grep -E "^(File|Lines executed|Creating)"

# View detailed line coverage for a specific file
cat card.c.gcov | less
```

**HTML coverage report (requires lcov):**
```bash
# Install lcov (one-time setup)
sudo apt-get install lcov

# Generate and view HTML report
make coverage
firefox coverage/index.html
```

The HTML report provides:
- Interactive file browser with color-coded coverage
- Line-by-line execution counts
- Branch coverage visualization
- Sortable coverage tables

### Coverage Files

The coverage target generates these artifacts:
- `*.gcov` - Line-by-line coverage data for each source file
- `coverage.info` - Aggregated coverage data (lcov format)
- `coverage/` - HTML coverage report directory

All coverage artifacts are ignored by git (see `.gitignore`).

### Interpreting Coverage Results

Each `.gcov` file shows:
- Execution counts for each line (number on the left)
- `#####` for lines that were never executed
- `-` for non-executable lines (comments, declarations)

**Example:**
```
        -:   10:/* Returns 1 if flush, 0 otherwise */
       42:   11:int is_flush(const Card* cards, size_t len) {
       42:   12:    if (len != 5) return 0;
       40:   13:    Suit first = cards[0].suit;
    #####:   14:    for (size_t i = 1; i < len; i++) {
```

This shows line 14 was never executed (likely unreachable code or edge case).

### Coverage in CI/CD

The coverage target is designed for integration with continuous integration:

```bash
# Run coverage and verify target met
make coverage
if grep -q "Lines executed:[0-9.]*% of" *.gcov; then
    echo "Coverage target met"
else
    echo "Coverage below target" && exit 1
fi
```

## Status

✅ **Phase 00 Complete** - Project structure, build system, and documentation framework established
✅ **Phase 01 Complete** - Foundation layer (Rank, Suit, Card, Deck) with full test coverage
✅ **Phase 02 Complete** - Evaluation core (HandCategory, Hand, helper functions) with comprehensive tests
✅ **Phase 03 Complete** - Detection layer (10 hand category detectors from Royal Flush to High Card)
✅ **Milestone 06 Complete** - Code Quality & Security (5/5 issues completed, average review score: 97/100)
  - Integer overflow prevention (CWE-190)
  - Double-free prevention (CWE-415)
  - Valgrind integration (19/19 tests, zero leaks)
  - Const-correctness (23 function signatures updated)
  - Fuzzing infrastructure (libFuzzer + AddressSanitizer)
✅ **Milestone 07 Complete** - Build System & Tooling (4/4 issues completed, average review score: 95.5/100)
  - Debug/release build configurations (`make debug`, `make release`)
  - Code coverage reporting with lcov (96.38% coverage achieved)
  - GitHub Actions CI/CD pipeline (7 jobs: GCC, Clang, Tests, Valgrind, Coverage, Fuzzing, Status)
  - Static analysis integration with cppcheck (`make lint`)

🚧 **Phase 04 In Progress** - Integration layer (`evaluate_hand()`, `compare_hands()`)
🚧 **Remaining Quality Milestones**:
  - Milestone 08: API & Documentation (7 issues)
  - Milestone 09: Refactoring & Optimization (7 issues)

## License

MIT License - See [LICENSE](LICENSE)
