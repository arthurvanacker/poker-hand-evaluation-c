# Poker Hand Evaluator (C)

A high-performance Texas Hold'em poker hand evaluator written in C with a focus on portability, memory safety, and performance.

## Project Vision

This project implements a complete poker hand evaluation system for Texas Hold'em, supporting 5, 6, and 7-card hand evaluation. Built with manual memory management and optimized for speed, it serves as both a practical library and an educational resource for understanding poker algorithms and C programming patterns.

## Planned Features

- **5-Card Evaluation**: Direct evaluation of standard poker hands
- **6-Card Evaluation**: Find best 5-card hand from 6 cards (6 combinations)
- **7-Card Evaluation**: Find best 5-card hand from 7 cards (21 combinations, Texas Hold'em)
- **Full Hand Detection**: All 10 poker hand categories (Royal Flush → High Card)
- **Tiebreaker System**: Complete comparison logic for determining winners
- **Memory Safe**: Valgrind-verified zero-leak implementation
- **Portable**: Standard C99 with no platform-specific dependencies
- **Well-Tested**: Comprehensive test coverage with Unity framework

## Architecture

The project is built in 5 phases:

### Phase 00: Project Initialization
Project structure, build system, and documentation setup.

### Phase 01: Foundation Layer
Core data structures (Card, Deck, Rank, Suit) and basic operations.

### Phase 02: Evaluation Core
Hand representation, helper functions (flush/straight detection, rank counting).

### Phase 03: Detection Layer
10 detector functions for all poker hand categories:
- Royal Flush
- Straight Flush
- Four of a Kind
- Full House
- Flush
- Straight
- Three of a Kind
- Two Pair
- One Pair
- High Card

### Phase 04: Integration Layer
Orchestration functions: `evaluate_hand()`, `compare_hands()`, `find_best_hand()`.

### Phase 05: Examples & Polish
Example programs, benchmarks, memory safety verification, and comprehensive documentation.

## C-Specific Features

- **Manual Memory Management**: Explicit `malloc()`/`free()` patterns with Valgrind verification
- **Output Parameters**: C-style error handling with return codes and output pointers
- **Fixed-Size Arrays**: Minimizes dynamic allocation for performance
- **Standard Library**: Uses `qsort()` for sorting, `memcpy()` for efficient copying
- **Array-Based Counting**: Direct rank indexing instead of hash tables
- **Portability**: C99 standard, no external dependencies (except Unity for testing)

## Project Status

🚧 **Under Development** 🚧

This README will be updated as each phase completes. Detailed usage examples and API documentation will be added incrementally.

## Documentation

Future documentation will include:
- `docs/ALGORITHM_EXPLAINED.md` - Comprehensive algorithm guide for beginners
- `analysis/MEMORY_SAFETY.md` - Memory safety patterns and Valgrind usage
- API reference with complete usage examples

## Building

```bash
make all      # Build static library
make test     # Run tests
make examples # Build example programs
make clean    # Remove build artifacts
```

## License

MIT License - See [LICENSE](LICENSE) file for details
