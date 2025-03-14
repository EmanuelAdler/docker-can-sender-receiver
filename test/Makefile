CC = gcc
CFLAGS = -Wall -Wextra -g --coverage
LDFLAGS = -lcunit --coverage

SRC = ../src/can_socket.c test_can_socket.c
SEND_SRC = ../src/sender.c
OBJ_DIR = obj
BIN_DIR = bin
TARGET = $(BIN_DIR)/test_can_socket
SEND_TARGET = $(BIN_DIR)/sender
OBJ = $(patsubst %.c, $(OBJ_DIR)/%.o, $(notdir $(SRC)))
SEND_OBJ = $(OBJ_DIR)/sender.o $(OBJ_DIR)/can_socket.o

all: $(TARGET) $(SEND_TARGET)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(TARGET): $(OBJ) | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(SEND_TARGET): $(SEND_OBJ) | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(OBJ_DIR)/%.o: ../src/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: %.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/sender.o: ../src/sender.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

test: all
	./$(TARGET)

send: $(SEND_TARGET)
	./$(SEND_TARGET)

coverage: test
	gcov -o $(OBJ_DIR) ../src/can_socket.c
	lcov --rc branch_coverage=1 --capture --directory $(OBJ_DIR) --output-file coverage.info
	genhtml --branch-coverage coverage.info --output-directory coverage_report
	@echo "Open coverage_report/index.html to view coverage results."

clean:
	rm -rf $(BIN_DIR) $(OBJ_DIR) *.gcda *.gcno *.gcov coverage.info
	rm -rf coverage_report

.PHONY: all test send coverage clean
