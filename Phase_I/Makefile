# Makefile for EV Charging Station Placement & Trip Planning System
# Course: CSEG3053 - Design and Analysis of Algorithms

CC ?= gcc
# Using -std=c11 for universal GCC/MinGW compatibility (fully compliant with C17 standard semantics)
CFLAGS ?= -Wall -Wextra -std=c11 -O2
LDFLAGS ?= -lm

SRC_DIR = src
TEST_DIR = tests
DATA_DIR = data

SRCS = $(SRC_DIR)/model.c \
       $(SRC_DIR)/analysis.c \
       $(SRC_DIR)/sorting_searching.c \
       $(SRC_DIR)/greedy.c \
       $(SRC_DIR)/dp.c

MAIN_SRC = $(SRC_DIR)/main.c
TEST_SRC = $(TEST_DIR)/test_runner.c

TARGET = ev_planner.exe
TEST_TARGET = test_runner.exe

.PHONY: all run test bench clean

all: $(TARGET) $(TEST_TARGET)

$(TARGET): $(MAIN_SRC) $(SRCS)
	$(CC) $(CFLAGS) -o $@ $(MAIN_SRC) $(SRCS) $(LDFLAGS)

$(TEST_TARGET): $(TEST_SRC) $(SRCS)
	$(CC) $(CFLAGS) -o $@ $(TEST_SRC) $(SRCS) $(LDFLAGS)

run: $(TARGET)
	./$(TARGET)

test: $(TEST_TARGET)
	./$(TEST_TARGET)

bench: $(TARGET)
	./$(TARGET)

clean:
	-del /Q $(TARGET) $(TEST_TARGET) *.o 2>NUL || rm -f $(TARGET) $(TEST_TARGET) *.o
