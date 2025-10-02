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

## Status

🚧 Under Development - This README will be updated as each phase completes.

## License

MIT License - See [LICENSE](LICENSE)
