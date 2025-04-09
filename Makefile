CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2
INCLUDE = -Iinclude
LIB = -lssl -lcrypto

SRC = src/main.c src/cipher.c src/key_schedule.c src/utils.c
OBJ = $(patsubst src/%.c, build/%.o, $(SRC))
TARGET = bin/cipher

TEST_DIR = tests
TEST_SRC = $(wildcard $(TEST_DIR)/*.c)
TEST_BIN = $(patsubst $(TEST_DIR)/%.c, bin/test/%, $(TEST_SRC))
SRC_NO_MAIN = $(filter-out src/main.c, $(SRC))
SRC_NO_MAIN_OBJ = $(patsubst src/%.c, build/%.o, $(SRC_NO_MAIN))

all: $(TARGET)

$(TARGET): $(OBJ)
	@mkdir -p bin
	$(CC) $(CFLAGS) $(OBJ) -o $@ $(LIB)
	@echo "[#] Build complete. Executable is located in the bin directory."

build/%.o: src/%.c
	@mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

test: $(TEST_BIN)
	@for t in $(TEST_BIN); do \
		echo "[#] Running $$t..."; \
		./$$t; \
	done

bin/test/%: tests/%.c $(SRC_NO_MAIN_OBJ)
	@mkdir -p bin/test
	$(CC) $(CFLAGS) $(INCLUDE) $< $(SRC_NO_MAIN_OBJ) -o $@ $(LIB)

clean:
	rm -rf build bin
