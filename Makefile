# Makefile for Poker Hand Evaluation Library
# Author: Generated for Issue #2
# Purpose: Build system for libpoker.a static library

# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -Iinclude
AR = ar
ARFLAGS = rcs

# Directories
SRC_DIR = src
BUILD_DIR = build
LIB_DIR = lib
INCLUDE_DIR = include
TEST_DIR = tests
EXAMPLES_DIR = examples
FUZZ_DIR = fuzz

# Source files
SRC = src/card.c src/deck.c src/evaluator.c

# Object files (convert src/*.c to build/*.o)
OBJ = $(SRC:src/%.c=build/%.o)

# Library output
LIB = lib/libpoker.a

# Default target
.PHONY: all
all: $(LIB)

# Build static library
$(LIB): $(OBJ) | $(LIB_DIR)
	$(AR) $(ARFLAGS) $@ $^
	@echo "Built library: $(LIB)"

# Build object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@
	@echo "Compiled: $<"

# Create directories if they don't exist
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(LIB_DIR):
	mkdir -p $(LIB_DIR)

# Test target - build and run tests
.PHONY: test
test: all
	@echo "Running tests..."
	@if [ -f $(TEST_DIR)/test_makefile.sh ]; then \
		$(TEST_DIR)/test_makefile.sh; \
	else \
		echo "No test files found"; \
	fi

# Examples target - build example programs
.PHONY: examples
examples: all
	@echo "Building examples..."
	@echo "No example programs yet"

# Install target - install library and headers
.PHONY: install
install: all
	@echo "Installing library and headers..."
	@echo "Install location: /usr/local/lib and /usr/local/include"
	@echo "Note: This is a placeholder - run with sudo for actual installation"
	# Uncomment for actual installation:
	# install -d /usr/local/lib
	# install -m 644 $(LIB) /usr/local/lib/
	# install -d /usr/local/include/poker
	# install -m 644 $(INCLUDE_DIR)/*.h /usr/local/include/poker/

# Clean target - remove build artifacts
.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)/*.o
	rm -rf $(LIB)
	@echo "Cleaned build artifacts"

# Valgrind target - memory leak verification
.PHONY: valgrind
valgrind: all
	@echo "Running Valgrind memory leak verification..."
	@echo "=============================================="
	@echo ""
	@# List of all test files
	@TEST_FILES=$$(find $(TEST_DIR) -maxdepth 1 -name "test_*.c" -type f); \
	TOTAL=0; \
	PASSED=0; \
	FAILED=0; \
	for test_file in $$TEST_FILES; do \
		test_name=$$(basename $$test_file .c); \
		test_exe=$(BUILD_DIR)/$$test_name; \
		echo "Testing: $$test_name"; \
		echo "----------------------------------------"; \
		if [ ! -f $$test_exe ]; then \
			echo "Building $$test_exe..."; \
			$(CC) $(CFLAGS) $$test_file $(LIB) -o $$test_exe 2>&1 | head -20; \
			if [ $$? -ne 0 ]; then \
				echo "✗ Build failed for $$test_name"; \
				echo ""; \
				FAILED=$$((FAILED + 1)); \
				continue; \
			fi; \
		fi; \
		valgrind --leak-check=full \
		         --show-leak-kinds=all \
		         --track-origins=yes \
		         --error-exitcode=1 \
		         --quiet \
		         $$test_exe > /dev/null 2>&1; \
		if [ $$? -eq 0 ]; then \
			echo "✓ $$test_name - No memory leaks"; \
			PASSED=$$((PASSED + 1)); \
		else \
			echo "✗ $$test_name - Memory leaks detected"; \
			echo "  Run manually: valgrind --leak-check=full $$test_exe"; \
			FAILED=$$((FAILED + 1)); \
		fi; \
		TOTAL=$$((TOTAL + 1)); \
		echo ""; \
	done; \
	echo "==============================================";\
	echo "Valgrind Summary: $$PASSED/$$TOTAL tests passed"; \
	if [ $$FAILED -gt 0 ]; then \
		echo "❌ $$FAILED test(s) failed - memory leaks detected"; \
		exit 1; \
	else \
		echo "✅ All tests passed - zero memory leaks!"; \
	fi

# Fuzzing targets - Security hardening with fuzzing
.PHONY: fuzz-standalone
fuzz-standalone: all
	@echo "Building fuzzing harnesses (standalone mode)..."
	@echo "=============================================="
	@echo ""
	@# Build fuzz_parse_card
	@echo "Building fuzz_parse_card..."
	$(CC) $(CFLAGS) -DFUZZ_STANDALONE $(FUZZ_DIR)/fuzz_parse_card.c $(LIB) -o $(BUILD_DIR)/fuzz_parse_card
	@echo "✓ Built: $(BUILD_DIR)/fuzz_parse_card"
	@echo ""
	@# Build fuzz_evaluate_hand
	@echo "Building fuzz_evaluate_hand..."
	$(CC) $(CFLAGS) -DFUZZ_STANDALONE $(FUZZ_DIR)/fuzz_evaluate_hand.c $(LIB) -o $(BUILD_DIR)/fuzz_evaluate_hand
	@echo "✓ Built: $(BUILD_DIR)/fuzz_evaluate_hand"
	@echo ""
	@echo "=============================================="
	@echo "Fuzzing harnesses built successfully!"
	@echo ""
	@echo "Run fuzzing tests:"
	@echo "  $(BUILD_DIR)/fuzz_parse_card"
	@echo "  $(BUILD_DIR)/fuzz_evaluate_hand"

.PHONY: fuzz-libfuzzer
fuzz-libfuzzer: all
	@echo "Building fuzzing harnesses (libFuzzer mode)..."
	@echo "=============================================="
	@echo ""
	@echo "Note: Requires clang with libFuzzer support"
	@echo ""
	@# Check if clang is available
	@if ! command -v clang > /dev/null 2>&1; then \
		echo "❌ clang not found - install with: sudo apt-get install clang"; \
		echo ""; \
		echo "For now, use 'make fuzz-standalone' to build gcc-based fuzzers"; \
		exit 1; \
	fi
	@echo "Building fuzz_parse_card with libFuzzer..."
	clang -fsanitize=fuzzer,address,undefined -I$(INCLUDE_DIR) $(FUZZ_DIR)/fuzz_parse_card.c $(LIB) -o $(BUILD_DIR)/fuzz_parse_card_libfuzzer
	@echo "✓ Built: $(BUILD_DIR)/fuzz_parse_card_libfuzzer"
	@echo ""
	@echo "Building fuzz_evaluate_hand with libFuzzer..."
	clang -fsanitize=fuzzer,address,undefined -I$(INCLUDE_DIR) $(FUZZ_DIR)/fuzz_evaluate_hand.c $(LIB) -o $(BUILD_DIR)/fuzz_evaluate_hand_libfuzzer
	@echo "✓ Built: $(BUILD_DIR)/fuzz_evaluate_hand_libfuzzer"
	@echo ""
	@echo "=============================================="
	@echo "libFuzzer harnesses built successfully!"
	@echo ""
	@echo "Run fuzzing campaigns:"
	@echo "  $(BUILD_DIR)/fuzz_parse_card_libfuzzer -max_total_time=3600"
	@echo "  $(BUILD_DIR)/fuzz_evaluate_hand_libfuzzer -max_total_time=3600"

.PHONY: fuzz
fuzz: fuzz-standalone
	@echo ""
	@echo "Running fuzzing tests..."
	@echo "=============================================="
	@echo ""
	@echo "Testing fuzz_parse_card..."
	@echo "----------------------------------------"
	@$(BUILD_DIR)/fuzz_parse_card
	@echo ""
	@echo "Testing fuzz_evaluate_hand..."
	@echo "----------------------------------------"
	@$(BUILD_DIR)/fuzz_evaluate_hand
	@echo ""
	@echo "=============================================="
	@echo "✅ All fuzzing tests completed successfully!"

# Help target
.PHONY: help
help:
	@echo "Poker Hand Evaluation Library - Build System"
	@echo ""
	@echo "Available targets:"
	@echo "  all            - Build static library (default)"
	@echo "  test           - Build and run tests"
	@echo "  valgrind       - Run Valgrind memory leak verification on all tests"
	@echo "  fuzz           - Build and run fuzzing tests (standalone mode)"
	@echo "  fuzz-standalone- Build fuzzing harnesses with gcc (no libFuzzer)"
	@echo "  fuzz-libfuzzer - Build fuzzing harnesses with clang + libFuzzer"
	@echo "  examples       - Build example programs"
	@echo "  clean          - Remove build artifacts"
	@echo "  install        - Install library and headers"
	@echo "  help           - Display this help message"
