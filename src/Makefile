CC = gcc
CFLAGS = -Wall
BIN_DIR = ../bin
SRC_DIR = .
OBJ = $(BIN_DIR)/can_socket.o
LDLFLAGS = -lssl -lcrypto

# Executables
TARGETS = $(BIN_DIR)/sender $(BIN_DIR)/receiver

all: $(TARGETS)

# Ensure the bin directory exists
$(shell mkdir -p $(BIN_DIR))

# Dependency rules
$(BIN_DIR)/can_socket.o: $(SRC_DIR)/can_socket.c $(SRC_DIR)/can_socket.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)/can_socket.c -o $(BIN_DIR)/can_socket.o

$(BIN_DIR)/sender.o: $(SRC_DIR)/sender.c $(SRC_DIR)/can_socket.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)/sender.c -o $(BIN_DIR)/sender.o

$(BIN_DIR)/receiver.o: $(SRC_DIR)/receiver.c $(SRC_DIR)/can_socket.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)/receiver.c -o $(BIN_DIR)/receiver.o

# Build rules for executables
$(BIN_DIR)/sender: $(BIN_DIR)/sender.o $(OBJ)
	$(CC) $(CFLAGS) $(BIN_DIR)/sender.o $(OBJ) -o $(BIN_DIR)/sender $(LDLFLAGS)

$(BIN_DIR)/receiver: $(BIN_DIR)/receiver.o $(OBJ)
	$(CC) $(CFLAGS) $(BIN_DIR)/receiver.o $(OBJ) -o $(BIN_DIR)/receiver $(LDLFLAGS)

# Clean command
clean:
	rm -rf $(BIN_DIR)/*

# Run command
run: all
	$(BIN_DIR)/sender & $(BIN_DIR)/receiver