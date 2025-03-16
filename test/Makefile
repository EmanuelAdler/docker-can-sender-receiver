CC = gcc
CFLAGS = -Wall -Wextra -g --coverage
LDFLAGS = -lcunit -lssl -lcrypto --coverage

SRC = ../src/can_socket.c test_can_socket.c
OBJ_DIR = obj
BIN_DIR = bin
TARGET = $(BIN_DIR)/test_can_socket
OBJ = $(patsubst %.c, $(OBJ_DIR)/%.o, $(notdir $(SRC)))

all: $(TARGET)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(TARGET): $(OBJ) | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(OBJ_DIR)/%.o: ../src/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: %.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

test: all
	./$(TARGET)

coverage: test
	gcov -o $(OBJ_DIR) ../src/can_socket.c
	lcov --rc branch_coverage=1 --capture --directory $(OBJ_DIR) --output-file coverage.info
	genhtml --branch-coverage coverage.info --output-directory coverage_report
	@echo "Open coverage_report/index.html to view coverage results."

clean:
	rm -rf $(BIN_DIR) $(OBJ_DIR) *.gcda *.gcno *.gcov coverage.info
	rm -rf coverage_report

.PHONY: all test coverage clean
