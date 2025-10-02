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

# Help target
.PHONY: help
help:
	@echo "Poker Hand Evaluation Library - Build System"
	@echo ""
	@echo "Available targets:"
	@echo "  all       - Build static library (default)"
	@echo "  test      - Build and run tests"
	@echo "  examples  - Build example programs"
	@echo "  clean     - Remove build artifacts"
	@echo "  install   - Install library and headers"
	@echo "  help      - Display this help message"
