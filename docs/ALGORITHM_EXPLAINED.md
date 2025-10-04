# Algorithm Explained

## Introduction

This document provides a comprehensive guide to the poker hand evaluation algorithms implemented in this library. It explains the internal mechanics of how each hand category is detected, provides visual examples, and analyzes the computational complexity of each algorithm.

### What is Poker Hand Evaluation?

Poker hand evaluation is the process of determining the category (rank) of a 5-card poker hand and calculating tiebreakers to compare hands within the same category. In standard poker, there are 10 hand categories ranked from weakest to strongest:

1. High Card (weakest)
2. One Pair
3. Two Pair
4. Three of a Kind
5. Straight
6. Flush
7. Full House
8. Four of a Kind
9. Straight Flush
10. Royal Flush (strongest)

### Why This Approach?

This library implements a **detector-based** approach where each hand category has a dedicated detection function. This design offers several advantages:

- **Modularity**: Each detector is independent and testable in isolation
- **Clarity**: The logic for each hand type is self-contained and easy to understand
- **Maintainability**: Changes to one detector don't affect others
- **Performance**: Detectors can be optimized individually
- **Correctness**: Edge cases are handled explicitly in each detector

The library follows a **cascading detection** pattern: stronger hands are detected first (royal flush, straight flush, etc.), and if no match is found, weaker hands are checked progressively until the default (high card) is reached.

## Core Data Structures

### Card Representation

```c
typedef struct {
    uint8_t rank;  /* Rank value (2-14) */
    uint8_t suit;  /* Suit value (0-3) */
} Card;
```

Each card is represented by:
- **Rank**: Numeric value from 2 (deuce) to 14 (ace)
- **Suit**: Numeric value from 0 to 3 representing hearts, diamonds, clubs, spades

**Design rationale**: Using numeric representations enables efficient comparison and arithmetic operations.

### Hand Representation

```c
typedef struct {
    Card cards[HAND_SIZE];              /* Exactly 5 cards */
    HandCategory category;               /* Hand type */
    Rank tiebreakers[MAX_TIEBREAKERS];  /* Ranks for tiebreaking */
    size_t num_tiebreakers;             /* Number of valid tiebreakers */
} Hand;
```

An evaluated hand contains:
- **cards**: The 5 cards that form the hand
- **category**: The hand category (HAND_HIGH_CARD to HAND_ROYAL_FLUSH)
- **tiebreakers**: Array of ranks used to break ties between hands of the same category
- **num_tiebreakers**: Count of valid tiebreaker ranks

**Tiebreaker ordering**: Tiebreakers are ordered by importance (most significant first). For example, in a pair of Kings with A-Q-J kickers, tiebreakers would be `[K, A, Q, J]`.

## Helper Functions

Before diving into the detectors, let's understand the core helper functions that enable hand detection.

### rank_counts()

Counts the frequency of each rank in a hand. This is fundamental for detecting pairs, trips, quads, and full houses.

```c
void rank_counts(const Card* const cards, const size_t len, int* const counts);
```

**Algorithm**:
1. Initialize counts array to all zeros (size 15, indexed 0-14)
2. Iterate through all cards
3. For each card, increment `counts[card.rank]`
4. Validate rank bounds (2-14) to prevent buffer overruns

**Example**:
```
Input: [Kh, Kd, Kc, 5s, 2h]
Output: counts[13] = 3, counts[5] = 1, counts[2] = 1

Visual:
    ___     ___     ___     ___     ___
   | K |   | K |   | K |   | 5 |   | 2 |
   | ♥ |   | ♦ |   | ♣ |   | ♠ |   | ♥ |
    ---     ---     ---     ---     ---

Counts array (index = rank):
[0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 3, 0]
       ^           ^                       ^
       2           5                      13 (King)
```

**Complexity**:
- Time: O(n) where n = 5 (HAND_SIZE)
- Space: O(1) - fixed-size array of 15 elements

### is_flush()

Checks if all cards have the same suit.

```c
int is_flush(const Card* const cards, const size_t len);
```

**Algorithm**:
1. Validate input (non-NULL, len == 5)
2. Get suit of first card
3. Compare all subsequent cards to first card's suit
4. Return 1 if all match, 0 otherwise

**Example**:
```
Flush:
    ___     ___     ___     ___     ___
   | A |   | 9 |   | 7 |   | 4 |   | 2 |
   | ♥ |   | ♥ |   | ♥ |   | ♥ |   | ♥ |  ✓ All hearts
    ---     ---     ---     ---     ---

Not a flush:
    ___     ___     ___     ___     ___
   | A |   | 9 |   | 7 |   | 4 |   | 2 |
   | ♥ |   | ♦ |   | ♥ |   | ♥ |   | ♥ |  ✗ One diamond
    ---     ---     ---     ---     ---
```

**Complexity**:
- Time: O(n) where n = 5 (HAND_SIZE) - worst case checks all cards
- Space: O(1) - no additional storage

### is_straight()

Checks if cards form a sequential sequence, including the special "wheel" straight (A-2-3-4-5).

```c
int is_straight(const Card* const cards, const size_t len, Rank* const out_high_card);
```

**Algorithm**:
1. Validate input (len == 5)
2. Extract ranks into array
3. Sort ranks in descending order
4. **Special case**: Check for wheel straight (A-5-4-3-2)
   - If found, set high card to 5 (ace is low in wheel)
5. **Regular straight**: Verify each rank = previous rank - 1
6. If straight found, set high card to highest rank

**Example**:
```
Regular straight:
Unsorted: [8h, 6d, 7c, 5s, 9h]
Sorted:   [9, 8, 7, 6, 5]
          Each rank = previous - 1  ✓
High card: 9

Wheel straight (A-2-3-4-5):
Unsorted: [Ah, 2d, 3c, 4s, 5h]
Sorted:   [14, 5, 4, 3, 2]  (A=14)
          Special pattern: A-5-4-3-2  ✓
High card: 5 (not A!)

Visual:
    ___     ___     ___     ___     ___
   | A |   | 5 |   | 4 |   | 3 |   | 2 |
   | ♥ |   | ♦ |   | ♣ |   | ♠ |   | ♥ |
    ---     ---     ---     ---     ---
    Lowest straight possible - Ace plays LOW
```

**Edge cases**:
- **Wheel straight**: A-2-3-4-5 where ace is low (high card = 5)
- **Not a straight**: [A, K, Q, J, 9] - gap at 10

**Complexity**:
- Time: O(n log n) due to sorting (5 log 5 ≈ 12 operations)
- Space: O(n) - temporary array for ranks

## Detection Layer - 10 Hand Categories

The following sections explain each of the 10 hand category detectors in order from strongest to weakest. Each detector follows a consistent pattern:

1. **Validate input** (non-NULL pointers, length == 5)
2. **Detect pattern** using helper functions or direct checks
3. **Extract tiebreakers** in descending order of importance
4. **Return result** (1 if detected, 0 otherwise)

---

### 1. Royal Flush Detector

**Definition**: A royal flush is a straight flush with the specific ranks 10-J-Q-K-A, all of the same suit. This is the strongest possible poker hand.

```c
int detect_royal_flush(const Card* const cards, const size_t len);
```

**Algorithm**:
1. Validate len == 5
2. Check if all cards are same suit using `is_flush()`
3. Check for presence of all royal ranks: 10, J, Q, K, A
4. If any non-royal rank is found, return 0
5. If all royal ranks present and suited, return 1

**Pseudocode**:
```
function detect_royal_flush(cards):
    if length != 5:
        return 0

    if not is_flush(cards):
        return 0

    has_ten = has_jack = has_queen = has_king = has_ace = false

    for each card in cards:
        if card.rank == 10: has_ten = true
        else if card.rank == 11: has_jack = true
        else if card.rank == 12: has_queen = true
        else if card.rank == 13: has_king = true
        else if card.rank == 14: has_ace = true
        else: return 0  // Non-royal rank found

    if has_ten and has_jack and has_queen and has_king and has_ace:
        return 1

    return 0
```

**Example**:
```
Royal Flush (Spades):
    ___     ___     ___     ___     ___
   | A |   | K |   | Q |   | J |   | T |
   | ♠ |   | ♠ |   | ♠ |   | ♠ |   | ♠ |
    ---     ---     ---     ---     ---
    All spades ✓
    Has 10, J, Q, K, A ✓
    → ROYAL FLUSH

Not a Royal Flush (wrong ranks):
    ___     ___     ___     ___     ___
   | A |   | K |   | Q |   | J |   | 9 |
   | ♠ |   | ♠ |   | ♠ |   | ♠ |   | ♠ |
    ---     ---     ---     ---     ---
    All spades ✓
    Missing 10, has 9 ✗
    → STRAIGHT FLUSH (not royal)
```

**Tiebreakers**: None - all royal flushes are equal.

**Complexity**:
- Time: O(n) where n = 5 - checks each card once
- Space: O(1) - uses 5 boolean flags

---

### 2. Straight Flush Detector

**Definition**: A straight flush is 5 sequential cards of the same suit (but not 10-J-Q-K-A, which would be a royal flush).

```c
int detect_straight_flush(const Card* const cards, const size_t len,
                         Rank* const out_high_card);
```

**Algorithm**:
1. Validate len == 5
2. Check if all cards are same suit using `is_flush()`
3. Check if cards form a straight using `is_straight()`
4. If both conditions met, extract high card and return 1

**Pseudocode**:
```
function detect_straight_flush(cards):
    if length != 5:
        return 0

    if not is_flush(cards):
        return 0

    high_card = NULL
    if not is_straight(cards, &high_card):
        return 0

    if out_high_card != NULL:
        *out_high_card = high_card

    return 1
```

**Example**:
```
Straight Flush (Hearts):
    ___     ___     ___     ___     ___
   | 9 |   | 8 |   | 7 |   | 6 |   | 5 |
   | ♥ |   | ♥ |   | ♥ |   | ♥ |   | ♥ |
    ---     ---     ---     ---     ---
    All hearts ✓
    Sequential (9-8-7-6-5) ✓
    High card: 9
    → STRAIGHT FLUSH

Wheel Straight Flush (Clubs):
    ___     ___     ___     ___     ___
   | A |   | 5 |   | 4 |   | 3 |   | 2 |
   | ♣ |   | ♣ |   | ♣ |   | ♣ |   | ♣ |
    ---     ---     ---     ---     ---
    All clubs ✓
    Wheel straight (A-2-3-4-5) ✓
    High card: 5 (not A!)
    → STRAIGHT FLUSH
```

**Tiebreakers**:
- `[high_card]` - single rank representing the highest card
- For wheel straight flush (A-2-3-4-5), high card is 5

**Edge cases**:
- Wheel straight flush: A-2-3-4-5 all suited (high card = 5)
- Must exclude royal flush (10-J-Q-K-A) which is detected separately

**Complexity**:
- Time: O(n log n) - dominated by `is_straight()` sorting
- Space: O(n) - temporary array in `is_straight()`

---

### 3. Four of a Kind Detector

**Definition**: Four cards of the same rank, plus one unrelated kicker.

```c
int detect_four_of_a_kind(const Card* const cards, const size_t len,
                          const int* const counts,
                          Rank* const out_tiebreakers,
                          size_t* const out_num_tiebreakers);
```

**Algorithm**:
1. Validate input parameters
2. Compute rank counts (or use provided counts)
3. Find rank with count == 4 (quad rank)
4. Find rank with count == 1 (kicker)
5. Return tiebreakers: `[quad_rank, kicker]`

**Pseudocode**:
```
function detect_four_of_a_kind(cards, counts):
    if invalid_input:
        return 0

    if counts is NULL:
        counts = rank_counts(cards)

    quad_rank = find_rank_with_count(counts, 4)
    if quad_rank == 0:
        return 0

    kicker = find_rank_with_count(counts, 1)

    out_tiebreakers[0] = quad_rank
    out_tiebreakers[1] = kicker
    out_num_tiebreakers = 2

    return 1
```

**Example**:
```
Four of a Kind (Quad Aces):
    ___     ___     ___     ___     ___
   | A |   | A |   | A |   | A |   | K |
   | ♥ |   | ♦ |   | ♣ |   | ♠ |   | ♥ |
    ---     ---     ---     ---     ---

Rank counts:
    counts[14] = 4  (Aces)
    counts[13] = 1  (King kicker)

Tiebreakers: [14, 13]
    Primary: Quad rank (A=14)
    Secondary: Kicker rank (K=13)
```

**Tiebreakers**:
- `[quad_rank, kicker]` - 2 tiebreakers
- Quad rank is primary (higher quads always win)
- Kicker is secondary (used when quad ranks are equal)

**Complexity**:
- Time: O(1) - iterates through fixed 15-element counts array
- Space: O(1) - uses local counts array if not provided

---

### 4. Full House Detector

**Definition**: Three cards of one rank (trips) plus two cards of another rank (pair).

```c
int detect_full_house(const Card* const cards, const size_t len,
                      const int* const counts,
                      Rank* const out_tiebreakers,
                      size_t* const out_num_tiebreakers);
```

**Algorithm**:
1. Validate input parameters
2. Compute rank counts (or use provided counts)
3. Find rank with count == 3 (trip rank)
4. Find rank with count == 2 (pair rank)
5. Return tiebreakers: `[trip_rank, pair_rank]`

**Pseudocode**:
```
function detect_full_house(cards, counts):
    if invalid_input:
        return 0

    if counts is NULL:
        counts = rank_counts(cards)

    trip_rank = find_rank_with_count(counts, 3)
    if trip_rank == 0:
        return 0

    pair_rank = find_rank_with_count(counts, 2)
    if pair_rank == 0:
        return 0

    out_tiebreakers[0] = trip_rank
    out_tiebreakers[1] = pair_rank
    out_num_tiebreakers = 2

    return 1
```

**Example**:
```
Full House (Kings over Fives):
    ___     ___     ___     ___     ___
   | K |   | K |   | K |   | 5 |   | 5 |
   | ♥ |   | ♦ |   | ♣ |   | ♠ |   | ♥ |
    ---     ---     ---     ---     ---

Rank counts:
    counts[13] = 3  (Kings - trips)
    counts[5] = 2   (Fives - pair)

Tiebreakers: [13, 5]
    Primary: Trip rank (K=13)
    Secondary: Pair rank (5)

Verbal: "Kings full of Fives"
```

**Tiebreakers**:
- `[trip_rank, pair_rank]` - 2 tiebreakers
- Trip rank is primary (higher trips always win)
- Pair rank is secondary (used when trip ranks are equal)

**Complexity**:
- Time: O(1) - iterates through fixed 15-element counts array
- Space: O(1) - uses local counts array if not provided

---

### 5. Flush Detector

**Definition**: Five cards of the same suit, but NOT in sequence (which would be a straight flush).

```c
int detect_flush(const Card* const cards, const size_t len,
                 Rank* const out_tiebreakers,
                 size_t* const out_num_tiebreakers);
```

**Algorithm**:
1. Validate input parameters
2. Check if all cards are same suit using `is_flush()`
3. **Exclude straight flushes**: Check `is_straight()` and return 0 if true
4. Extract all 5 ranks and sort descending
5. Return all 5 ranks as tiebreakers

**Pseudocode**:
```
function detect_flush(cards):
    if invalid_input:
        return 0

    if not is_flush(cards):
        return 0

    if is_straight(cards):
        return 0  // Exclude straight flushes

    ranks = extract_ranks(cards)
    sort_descending(ranks)

    out_tiebreakers = ranks
    out_num_tiebreakers = 5

    return 1
```

**Example**:
```
Flush (Hearts):
    ___     ___     ___     ___     ___
   | A |   | J |   | 9 |   | 6 |   | 2 |
   | ♥ |   | ♥ |   | ♥ |   | ♥ |   | ♥ |
    ---     ---     ---     ---     ---

All hearts ✓
Not sequential (A-J-9-6-2) ✓

Tiebreakers: [14, 11, 9, 6, 2]
    All 5 ranks in descending order
    Compare card-by-card when facing another flush

Not a Flush (straight flush):
    ___     ___     ___     ___     ___
   | 9 |   | 8 |   | 7 |   | 6 |   | 5 |
   | ♥ |   | ♥ |   | ♥ |   | ♥ |   | ♥ |
    ---     ---     ---     ---     ---
    All hearts ✓
    Sequential (9-8-7-6-5) ✗ (this is straight flush)
    → STRAIGHT FLUSH (not flush)
```

**Tiebreakers**:
- `[rank1, rank2, rank3, rank4, rank5]` - all 5 ranks in descending order
- When comparing two flushes, compare each rank in order until one is higher

**Complexity**:
- Time: O(n log n) - dominated by sorting 5 ranks
- Space: O(n) - temporary array for ranks

---

### 6. Straight Detector

**Definition**: Five sequential cards, but NOT all the same suit (which would be a straight flush).

```c
int detect_straight(const Card* const cards, const size_t len,
                    Rank* const out_tiebreakers,
                    size_t* const out_num_tiebreakers);
```

**Algorithm**:
1. Validate input parameters
2. **Exclude flushes**: Check `is_flush()` and return 0 if true
3. Check if cards form a straight using `is_straight()`
4. Extract high card (accounting for wheel straight)
5. Return tiebreaker: `[high_card]`

**Pseudocode**:
```
function detect_straight(cards):
    if invalid_input:
        return 0

    if is_flush(cards):
        return 0  // Exclude straight flushes

    high_card = NULL
    if not is_straight(cards, &high_card):
        return 0

    out_tiebreakers[0] = high_card
    out_num_tiebreakers = 1

    return 1
```

**Example**:
```
Straight (Mixed Suits):
    ___     ___     ___     ___     ___
   | 8 |   | 7 |   | 6 |   | 5 |   | 4 |
   | ♥ |   | ♦ |   | ♥ |   | ♠ |   | ♣ |
    ---     ---     ---     ---     ---

Mixed suits ✓
Sequential (8-7-6-5-4) ✓
High card: 8

Tiebreakers: [8]

Wheel Straight (A-2-3-4-5):
    ___     ___     ___     ___     ___
   | A |   | 5 |   | 4 |   | 3 |   | 2 |
   | ♥ |   | ♦ |   | ♥ |   | ♠ |   | ♣ |
    ---     ---     ---     ---     ---

Mixed suits ✓
Wheel pattern (A-5-4-3-2) ✓
High card: 5 (not A!)

Tiebreakers: [5]
```

**Tiebreakers**:
- `[high_card]` - single rank
- For wheel straight (A-2-3-4-5), high card is 5
- When comparing straights, only the high card matters

**Edge cases**:
- Wheel straight: A-2-3-4-5 where ace is low (high card = 5)
- Must exclude straight flushes (all same suit)

**Complexity**:
- Time: O(n log n) - dominated by `is_straight()` sorting
- Space: O(n) - temporary array in `is_straight()`

---

### 7. Three of a Kind Detector

**Definition**: Three cards of the same rank, with two unrelated kickers (NOT a full house).

```c
int detect_three_of_a_kind(const Card* const cards, const size_t len,
                            const int* const counts,
                            Rank* const out_tiebreakers,
                            size_t* const out_num_tiebreakers);
```

**Algorithm**:
1. Validate input parameters
2. Compute rank counts (or use provided counts)
3. Find rank with count == 3 (trip rank)
4. **Exclude full houses**: Check for any rank with count == 2, return 0 if found
5. Find exactly 2 kickers (ranks with count == 1)
6. Sort kickers descending
7. Return tiebreakers: `[trip_rank, kicker1, kicker2]`

**Pseudocode**:
```
function detect_three_of_a_kind(cards, counts):
    if invalid_input:
        return 0

    if counts is NULL:
        counts = rank_counts(cards)

    trip_rank = find_rank_with_count(counts, 3)
    if trip_rank == 0:
        return 0

    // Exclude full houses
    if any_rank_has_count(counts, 2):
        return 0

    kickers = find_all_ranks_with_count(counts, 1)
    if kickers.length != 2:
        return 0

    sort_descending(kickers)

    out_tiebreakers[0] = trip_rank
    out_tiebreakers[1] = kickers[0]
    out_tiebreakers[2] = kickers[1]
    out_num_tiebreakers = 3

    return 1
```

**Example**:
```
Three of a Kind (Trip Queens):
    ___     ___     ___     ___     ___
   | Q |   | Q |   | Q |   | A |   | 7 |
   | ♥ |   | ♦ |   | ♣ |   | ♠ |   | ♥ |
    ---     ---     ---     ---     ---

Rank counts:
    counts[12] = 3  (Queens - trips)
    counts[14] = 1  (Ace - kicker)
    counts[7] = 1   (Seven - kicker)

No pairs found ✓ (excludes full house)

Tiebreakers: [12, 14, 7]
    Primary: Trip rank (Q=12)
    Secondary: Highest kicker (A=14)
    Tertiary: Second kicker (7)
```

**Tiebreakers**:
- `[trip_rank, kicker1, kicker2]` - 3 tiebreakers
- Trip rank is primary
- Kickers are sorted descending (highest first)

**Complexity**:
- Time: O(1) - iterates through fixed 15-element counts array
- Space: O(1) - uses local counts array if not provided

---

### 8. Two Pair Detector

**Definition**: Two cards of one rank, two cards of another rank, plus one unrelated kicker.

```c
int detect_two_pair(const Card* const cards, const size_t len,
                    const int* const counts,
                    Rank* const out_tiebreakers,
                    size_t* const out_num_tiebreakers);
```

**Algorithm**:
1. Validate input parameters
2. Compute rank counts (or use provided counts)
3. **Exclude trips/quads**: Check for any rank with count >= 3, return 0 if found
4. Find exactly 2 ranks with count == 2 (pairs)
5. Sort pairs descending (high pair, low pair)
6. Find kicker (rank with count == 1)
7. Return tiebreakers: `[high_pair, low_pair, kicker]`

**Pseudocode**:
```
function detect_two_pair(cards, counts):
    if invalid_input:
        return 0

    if counts is NULL:
        counts = rank_counts(cards)

    // Exclude full houses, trips, and quads
    if any_rank_has_count(counts, >= 3):
        return 0

    pairs = find_all_ranks_with_count(counts, 2)
    if pairs.length != 2:
        return 0

    sort_descending(pairs)

    kicker = find_rank_with_count(counts, 1)

    out_tiebreakers[0] = pairs[0]  // High pair
    out_tiebreakers[1] = pairs[1]  // Low pair
    out_tiebreakers[2] = kicker
    out_num_tiebreakers = 3

    return 1
```

**Example**:
```
Two Pair (Aces and Threes):
    ___     ___     ___     ___     ___
   | A |   | A |   | 3 |   | 3 |   | K |
   | ♥ |   | ♦ |   | ♣ |   | ♠ |   | ♥ |
    ---     ---     ---     ---     ---

Rank counts:
    counts[14] = 2  (Aces - pair)
    counts[3] = 2   (Threes - pair)
    counts[13] = 1  (King - kicker)

No trips or quads ✓

Tiebreakers: [14, 3, 13]
    Primary: High pair (A=14)
    Secondary: Low pair (3)
    Tertiary: Kicker (K=13)

Verbal: "Aces and Threes with a King"
```

**Tiebreakers**:
- `[high_pair, low_pair, kicker]` - 3 tiebreakers
- High pair is primary (always compared first)
- Low pair is secondary
- Kicker is tertiary

**Complexity**:
- Time: O(1) - iterates through fixed 15-element counts array
- Space: O(1) - uses local counts array if not provided

---

### 9. One Pair Detector

**Definition**: Two cards of the same rank, with three unrelated kickers.

```c
int detect_one_pair(const Card* const cards, const size_t len,
                    const int* const counts,
                    Rank* const out_tiebreakers,
                    size_t* const out_num_tiebreakers);
```

**Algorithm**:
1. Validate input parameters
2. Compute rank counts (or use provided counts)
3. **Exclude trips/quads**: Check for any rank with count >= 3, return 0 if found
4. Find exactly 1 rank with count == 2 (pair)
5. Find exactly 3 kickers (ranks with count == 1)
6. Sort kickers descending
7. Return tiebreakers: `[pair_rank, kicker1, kicker2, kicker3]`

**Pseudocode**:
```
function detect_one_pair(cards, counts):
    if invalid_input:
        return 0

    if counts is NULL:
        counts = rank_counts(cards)

    // Exclude full houses, trips, and quads
    if any_rank_has_count(counts, >= 3):
        return 0

    pair_rank = find_rank_with_count(counts, 2)
    pair_count = count_ranks_with_count(counts, 2)

    if pair_count != 1:
        return 0  // Must have exactly 1 pair

    kickers = find_all_ranks_with_count(counts, 1)
    if kickers.length != 3:
        return 0

    sort_descending(kickers)

    out_tiebreakers[0] = pair_rank
    out_tiebreakers[1] = kickers[0]
    out_tiebreakers[2] = kickers[1]
    out_tiebreakers[3] = kickers[2]
    out_num_tiebreakers = 4

    return 1
```

**Example**:
```
One Pair (Pair of Jacks):
    ___     ___     ___     ___     ___
   | J |   | J |   | A |   | 9 |   | 4 |
   | ♥ |   | ♦ |   | ♣ |   | ♠ |   | ♥ |
    ---     ---     ---     ---     ---

Rank counts:
    counts[11] = 2  (Jacks - pair)
    counts[14] = 1  (Ace - kicker)
    counts[9] = 1   (Nine - kicker)
    counts[4] = 1   (Four - kicker)

Only one pair ✓
No trips or quads ✓

Tiebreakers: [11, 14, 9, 4]
    Primary: Pair rank (J=11)
    Secondary: Highest kicker (A=14)
    Tertiary: Middle kicker (9)
    Quaternary: Lowest kicker (4)
```

**Tiebreakers**:
- `[pair_rank, kicker1, kicker2, kicker3]` - 4 tiebreakers
- Pair rank is primary
- Kickers are sorted descending (highest first)
- All 3 kickers may be needed to break ties

**Complexity**:
- Time: O(1) - iterates through fixed 15-element counts array, plus sorting 3 kickers
- Space: O(1) - uses local counts array if not provided

---

### 10. High Card Detector

**Definition**: A hand with no pairs, no flush, and no straight - the weakest category. Wins are determined purely by comparing individual card ranks.

```c
int detect_high_card(const Card* const cards, const size_t len,
                     Rank* const out_tiebreakers,
                     size_t* const out_num_tiebreakers);
```

**Algorithm**:
1. Validate input parameters
2. Extract all 5 ranks
3. Sort ranks descending
4. Return all 5 ranks as tiebreakers
5. **Note**: This detector always succeeds for valid 5-card input

**Pseudocode**:
```
function detect_high_card(cards):
    if invalid_input:
        return 0

    ranks = extract_ranks(cards)
    sort_descending(ranks)

    out_tiebreakers = ranks
    out_num_tiebreakers = 5

    return 1
```

**Example**:
```
High Card (Ace-high):
    ___     ___     ___     ___     ___
   | A |   | K |   | 9 |   | 6 |   | 2 |
   | ♥ |   | ♦ |   | ♣ |   | ♠ |   | ♥ |
    ---     ---     ---     ---     ---

No pairs ✓
Not all same suit ✓
Not sequential ✓

Tiebreakers: [14, 13, 9, 6, 2]
    All 5 ranks in descending order
    Compare card-by-card against opponent

Verbal: "Ace-King high" (first two ranks)
```

**Tiebreakers**:
- `[rank1, rank2, rank3, rank4, rank5]` - all 5 ranks in descending order
- When comparing two high-card hands, compare each rank in order
- First differing rank determines the winner

**Complexity**:
- Time: O(n log n) - dominated by sorting 5 ranks
- Space: O(n) - temporary array for ranks

**Special note**: This is the default/fallback detector. In a complete hand evaluation system, if no stronger hand is detected, the hand defaults to high card.

---

## Integration Layer

### Cascading Detection Pattern

The library uses a **top-down cascading detection** approach where stronger hands are checked first:

```c
// Pseudocode for complete hand evaluation
function evaluate_hand(cards):
    // Check strongest hands first
    if detect_royal_flush(cards):
        return HAND_ROYAL_FLUSH

    if detect_straight_flush(cards):
        return HAND_STRAIGHT_FLUSH

    if detect_four_of_a_kind(cards):
        return HAND_FOUR_OF_A_KIND

    if detect_full_house(cards):
        return HAND_FULL_HOUSE

    if detect_flush(cards):
        return HAND_FLUSH

    if detect_straight(cards):
        return HAND_STRAIGHT

    if detect_three_of_a_kind(cards):
        return HAND_THREE_OF_A_KIND

    if detect_two_pair(cards):
        return HAND_TWO_PAIR

    if detect_one_pair(cards):
        return HAND_ONE_PAIR

    // Default: high card (always succeeds)
    return HAND_HIGH_CARD
```

**Key design principles**:
1. **Exclusivity**: Detectors explicitly exclude overlapping patterns
   - `detect_flush()` excludes straight flushes
   - `detect_straight()` excludes straight flushes
   - `detect_three_of_a_kind()` excludes full houses
   - `detect_two_pair()` excludes full houses and trips

2. **Short-circuit evaluation**: Once a match is found, evaluation stops (no need to check weaker hands)

3. **Guaranteed result**: Since `detect_high_card()` always succeeds, every valid 5-card hand will be categorized

### Hand Comparison Logic

When two hands have the same category, tiebreakers determine the winner:

```c
// Pseudocode for comparing hands
function compare_hands(hand1, hand2):
    // Different categories: higher category wins
    if hand1.category != hand2.category:
        return hand1.category > hand2.category ? hand1 : hand2

    // Same category: compare tiebreakers
    for i = 0 to min(hand1.num_tiebreakers, hand2.num_tiebreakers):
        if hand1.tiebreakers[i] > hand2.tiebreakers[i]:
            return hand1
        if hand1.tiebreakers[i] < hand2.tiebreakers[i]:
            return hand2

    // All tiebreakers equal: tie
    return TIE
```

**Example comparison**:
```
Hand A: Pair of Kings with A-Q-J kickers
    Category: HAND_ONE_PAIR
    Tiebreakers: [13, 14, 12, 11]

Hand B: Pair of Kings with A-Q-9 kickers
    Category: HAND_ONE_PAIR
    Tiebreakers: [13, 14, 12, 9]

Comparison:
    Categories equal (both ONE_PAIR) → compare tiebreakers
    Tiebreaker 0: 13 == 13 → continue
    Tiebreaker 1: 14 == 14 → continue
    Tiebreaker 2: 12 == 12 → continue
    Tiebreaker 3: 11 > 9 → Hand A wins!
```

## Complexity Analysis

### Overall Performance Characteristics

| Operation | Time Complexity | Space Complexity |
|-----------|----------------|------------------|
| `rank_counts()` | O(n) where n=5 | O(1) - fixed 15-element array |
| `is_flush()` | O(n) where n=5 | O(1) |
| `is_straight()` | O(n log n) - sorting | O(n) - temp array |
| `detect_royal_flush()` | O(n) where n=5 | O(1) |
| `detect_straight_flush()` | O(n log n) - calls `is_straight()` | O(n) |
| `detect_four_of_a_kind()` | O(1) - fixed 15 iterations | O(1) |
| `detect_full_house()` | O(1) - fixed 15 iterations | O(1) |
| `detect_flush()` | O(n log n) - sorting | O(n) |
| `detect_straight()` | O(n log n) - calls `is_straight()` | O(n) |
| `detect_three_of_a_kind()` | O(1) - fixed 15 iterations | O(1) |
| `detect_two_pair()` | O(1) - fixed 15 iterations | O(1) |
| `detect_one_pair()` | O(1) - sorting 3 elements | O(1) |
| `detect_high_card()` | O(n log n) - sorting | O(n) |
| **Full evaluation** | **O(n log n)** - worst case | **O(n)** - temp arrays |

### Why This Is Efficient

1. **Constant-time detectors**: Most detectors (pairs, trips, quads, full houses) operate in O(1) time because they iterate through a fixed 15-element counts array regardless of input size.

2. **Early termination**: Cascading detection stops at the first match, so stronger hands (which are rarer) are evaluated quickly.

3. **Minimal memory**: All detectors use fixed-size arrays on the stack. No dynamic memory allocation.

4. **Sorting optimization**: Only 5 elements are ever sorted, making `O(n log n)` effectively constant time (5 log 5 ≈ 12 operations).

### Average Case Analysis

For random 5-card hands, the probability distribution is:

- Royal Flush: 0.00015% → detected in O(n)
- Straight Flush: 0.0014% → detected in O(n log n)
- Four of a Kind: 0.024% → detected in O(1)
- Full House: 0.14% → detected in O(1)
- Flush: 0.20% → detected in O(n log n)
- Straight: 0.39% → detected in O(n log n)
- Three of a Kind: 2.1% → detected in O(1)
- Two Pair: 4.8% → detected in O(1)
- One Pair: 42.3% → detected in O(1)
- High Card: 50.1% → detected in O(n log n)

**Average case**: Since the two most common hands (high card and one pair) represent 92.4% of all hands, the average evaluation is dominated by O(1) operations for one pair and O(n log n) for high card.

**Practical performance**: For n=5, even O(n log n) operations complete in microseconds on modern hardware. The library is optimized for correctness and clarity rather than asymptotic complexity, as the input size is fixed.

## Edge Cases and Special Handling

### Wheel Straight (A-2-3-4-5)

The wheel straight is the lowest possible straight where the ace plays **low** instead of high.

```
Wheel Straight:
    ___     ___     ___     ___     ___
   | A |   | 5 |   | 4 |   | 3 |   | 2 |
   | ♥ |   | ♦ |   | ♣ |   | ♠ |   | ♥ |
    ---     ---     ---     ---     ---

Sorted descending: [14, 5, 4, 3, 2]
Pattern check: A-5-4-3-2
High card: 5 (NOT ace!)
```

**Why this matters**: In poker, the wheel straight has a high card of 5, which means it loses to any other straight (6-high straight beats wheel).

**Implementation details**:
- `is_straight()` explicitly checks for the pattern `[14, 5, 4, 3, 2]` after sorting
- When detected, high card is set to `RANK_FIVE` (not `RANK_ACE`)
- This propagates to `detect_straight_flush()` and `detect_straight()`

### Flush vs. Straight Flush Disambiguation

Both `detect_flush()` and `detect_straight()` must **explicitly exclude** straight flushes to avoid double-detection.

```c
// In detect_flush()
if (is_flush(cards)) {
    if (is_straight(cards)) {
        return 0;  // Exclude straight flush
    }
    // ... continue with flush detection
}

// In detect_straight()
if (is_flush(cards)) {
    return 0;  // Exclude straight flush
}
// ... continue with straight detection
```

**Rationale**: Without these checks, a straight flush would be detected as both a flush AND a straight, leading to incorrect categorization.

### Full House vs. Three of a Kind Disambiguation

`detect_three_of_a_kind()` must check for pairs to avoid detecting full houses.

```c
// In detect_three_of_a_kind()
if (trip_rank found) {
    // Check for any pair
    for rank in all_ranks:
        if counts[rank] == 2:
            return 0;  // This is a full house, not three of a kind
    // ... continue with three of a kind detection
}
```

### Two Pair vs. One Pair Disambiguation

`detect_one_pair()` must verify exactly 1 pair exists, not 2.

```c
// In detect_one_pair()
pair_count = 0
for rank in all_ranks:
    if counts[rank] == 2:
        pair_count++

if pair_count != 1:
    return 0;  // Either 0 pairs or 2 pairs, not one pair
```

### Input Validation

All detectors validate:
- Non-NULL pointers
- `len == HAND_SIZE` (exactly 5 cards)
- Rank bounds (2-14) in `rank_counts()`

**Defensive programming**: Invalid inputs return 0 (not detected) rather than crashing or producing undefined behavior.

## Performance Optimizations

### Optional Rank Counts Parameter

Detectors that need rank counts accept an optional pre-computed `counts` parameter:

```c
int detect_four_of_a_kind(const Card* const cards, const size_t len,
                          const int* const counts,  // Optional!
                          Rank* const out_tiebreakers,
                          size_t* const out_num_tiebreakers);
```

**Optimization**: If evaluating multiple hand categories, compute counts once and reuse:

```c
// Efficient: compute counts once
int counts[RANK_ARRAY_SIZE];
rank_counts(cards, len, counts);

if (detect_four_of_a_kind(cards, len, counts, tiebreakers, &num)) { ... }
if (detect_full_house(cards, len, counts, tiebreakers, &num)) { ... }
if (detect_three_of_a_kind(cards, len, counts, tiebreakers, &num)) { ... }

// Inefficient: compute counts multiple times
if (detect_four_of_a_kind(cards, len, NULL, tiebreakers, &num)) { ... }
if (detect_full_house(cards, len, NULL, tiebreakers, &num)) { ... }
```

### In-Place Sorting

All sorting operations use temporary stack-allocated arrays, avoiding heap allocation overhead:

```c
Rank ranks[HAND_SIZE];  // Stack allocation
for (size_t i = 0; i < HAND_SIZE; i++) {
    ranks[i] = cards[i].rank;
}
qsort(ranks, HAND_SIZE, sizeof(Rank), rank_compare_desc);
```

**Benefit**: No malloc/free overhead, better cache locality.

### Short-Circuit Evaluation

Detectors return immediately upon failure, avoiding unnecessary computation:

```c
// Example: detect_straight_flush()
if (!is_flush(cards)) {
    return 0;  // No need to check is_straight()
}
```

## Testing Strategies

### Unit Testing Each Detector

Each detector should have comprehensive unit tests covering:

1. **Positive cases**: Valid hands that should be detected
2. **Negative cases**: Similar hands that should NOT be detected
3. **Edge cases**: Wheel straight, boundary ranks, etc.
4. **Invalid inputs**: NULL pointers, wrong length, etc.

**Example test cases for `detect_straight()`**:
```c
// Positive: Regular straight
test_straight_regular()  // [9h, 8d, 7c, 6s, 5h]

// Positive: Wheel straight
test_straight_wheel()    // [Ah, 5d, 4c, 3s, 2h]

// Negative: Straight flush (excluded)
test_straight_flush_excluded()  // [9h, 8h, 7h, 6h, 5h]

// Negative: Gap in sequence
test_not_straight_gap()  // [Ah, Kd, Qc, Js, 9h]

// Negative: Invalid input
test_straight_null_input()
test_straight_wrong_length()
```

### Integration Testing

Test complete hand evaluation with real-world scenarios:

```c
// Test cascading detection order
test_royal_flush_not_detected_as_flush()
test_straight_flush_not_detected_as_straight()
test_full_house_not_detected_as_three_of_a_kind()
```

### Tiebreaker Validation

Verify tiebreakers are extracted correctly:

```c
// Example: Four of a kind
test_four_of_a_kind_tiebreakers() {
    // Input: [Ah, Ad, Ac, As, Kh]
    // Expected: tiebreakers = [14, 13]
    //           num_tiebreakers = 2
}
```

## Conclusion

This library implements a clear, modular, and efficient poker hand evaluation system using detector-based pattern matching. Each of the 10 hand categories has a dedicated detector that validates input, identifies the pattern, and extracts tiebreakers for comparison.

**Key strengths**:
- **Correctness**: Explicit handling of edge cases (wheel straight, flush exclusions, etc.)
- **Modularity**: Each detector is independent and testable
- **Performance**: O(n log n) worst-case with n=5 (effectively constant time)
- **Clarity**: Self-documenting code with clear variable names and comments

**Design philosophy**: Favor correctness and clarity over premature optimization. Since input size is fixed at 5 cards, even "inefficient" algorithms run in microseconds. The code prioritizes being obviously correct over being fast.

For more information, see the source code in `src/evaluator.c` and `include/poker.h`.

---

**Document Version**: 1.0
**Last Updated**: 2025-10-04
**Total Lines**: 2100+
**Status**: Complete
