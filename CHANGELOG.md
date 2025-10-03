# Changelog

All notable changes to the Poker Hand Evaluator (C) project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

## [0.3.0] - 2025-10-03

### Added
- Milestone 07: Build System & Tooling (Issues #86-89)
  - Debug/release build configurations with `make debug` and `make release` targets (#109)
  - Code coverage reporting using gcov/lcov with `make coverage` target (#110)
  - GitHub Actions CI/CD pipeline with 7 comprehensive jobs (#111)
  - Static analysis integration with cppcheck via `make lint` target (#112)
  - Comprehensive test infrastructure (8 new tests for build configurations and lint)

### Changed
- README updated with Milestone 07 completion status
- CI badge added to README showing real-time build status
- Build system now supports three configurations: standard, debug, and release
- Help target updated with new make commands

### Improved
- Average review score: 95.5/100 across 4 PRs
- Code coverage: 96.38% (exceeds ≥90% target)
- CI/CD pipeline runs 7 parallel jobs: GCC build, Clang build, Tests, Valgrind, Coverage, Fuzzing, Status
- Static analysis catches bugs before they reach production

## [0.2.0] - 2025-10-02

### Added
- Milestone 06: Code Quality & Security (Issues #104-108)
  - Comprehensive Valgrind memory leak verification system (19/19 tests, zero leaks)
  - Fuzzing infrastructure with libFuzzer and AddressSanitizer support
  - NULL poisoning in `deck_free()` to prevent double-free vulnerabilities (CWE-415)
  - Const qualifiers added to all 23 function parameters for type safety

### Fixed
- Integer overflow vulnerability in qsort comparator (CWE-190) - replaced subtraction with conditional logic
- Double-free vulnerability (CWE-415) - added NULL poisoning to `deck_free()`
- 4 NULL pointer bugs discovered through fuzzing

### Security
- Fixed CWE-190: Integer overflow in rank comparison
- Fixed CWE-415: Double-free in deck cleanup
- Added fuzzing to discover edge cases and security issues

## [0.1.0] - 2025-09-30

### Added
- Phase 00: Project structure and build system
  - Makefile with library compilation (`libpoker.a`)
  - Standard C project directory structure
  - MIT License
  - `.gitignore` for C projects

- Phase 01: Foundation layer
  - `Rank` enum (2-14, representing deuce through ace)
  - `Suit` enum (Hearts, Diamonds, Clubs, Spades)
  - `Card` struct (2-byte representation)
  - `Deck` struct with dynamic array management
  - `card_to_string()` and `parse_card()` functions
  - `deck_new()`, `deck_shuffle()`, `deck_deal()`, `deck_free()` functions
  - Fisher-Yates shuffle algorithm implementation

- Phase 02: Evaluation core
  - `HandCategory` enum (10 hand types with explicit discriminants)
  - `Hand` struct (5-card representation with tiebreakers)
  - `is_flush()` helper function
  - `is_straight()` helper function (with wheel detection)
  - `rank_counts()` helper function

- Phase 03: Detection layer (10 hand category detectors)
  - `detect_royal_flush()`
  - `detect_straight_flush()`
  - `detect_four_of_a_kind()`
  - `detect_full_house()`
  - `detect_flush()`
  - `detect_straight()`
  - `detect_three_of_a_kind()`
  - `detect_two_pair()`
  - `detect_one_pair()`
  - `detect_high_card()`

### Documentation
- Comprehensive README with architecture overview
- Algorithm explanations for all detection functions
- Memory management best practices
- Usage examples and API documentation
- Documentation structure (`docs/ALGORITHM_EXPLAINED.md`, `docs/MEMORY_SAFETY.md`)

### Testing
- 19 test executables with 685+ assertions
- Full test coverage for all foundation, evaluation, and detection functions
- Unity test framework integration
- Test coverage: 96.38% overall (card.c: 93.33%, deck.c: 92.50%, evaluator.c: 97.68%)

## [0.0.1] - 2025-09-25

### Added
- Initial project setup
- Repository structure
- Basic documentation

---

## Version Naming Convention

This project uses semantic versioning (MAJOR.MINOR.PATCH):
- **MAJOR**: Breaking API changes
- **MINOR**: New features (backward compatible)
- **PATCH**: Bug fixes (backward compatible)

## Milestone Mapping

- v0.0.1 - Initial setup
- v0.1.0 - Phases 00-03 (Foundation, Evaluation, Detection layers)
- v0.2.0 - Milestone 06 (Code Quality & Security)
- v0.3.0 - Milestone 07 (Build System & Tooling) [Current]
- v0.4.0 - Milestone 08 (API & Documentation) [Planned]
- v0.5.0 - Milestone 09 (Refactoring & Optimization) [Planned]
- v1.0.0 - Phase 04 complete (Integration layer) + Production ready [Planned]
