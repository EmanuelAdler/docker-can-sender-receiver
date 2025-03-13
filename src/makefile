CC = gcc
CFLAGS = -Wall
TARGETS = sender receiver
OBJ = can_socket.o

all: $(TARGETS)

# Dependency rules
can_socket.o: can_socket.c can_socket.h
	$(CC) $(CFLAGS) -c can_socket.c -o can_socket.o

sender.o: sender.c can_socket.h
	$(CC) $(CFLAGS) -c sender.c -o sender.o

receiver.o: receiver.c can_socket.h
	$(CC) $(CFLAGS) -c receiver.c -o receiver.o

# Build rules for executables
sender: sender.o $(OBJ)
	$(CC) $(CFLAGS) sender.o $(OBJ) -o sender

receiver: receiver.o $(OBJ)
	$(CC) $(CFLAGS) receiver.o $(OBJ) -o receiver

# Clean command
clean:
	rm -f $(TARGETS) *.o

# Run command
run: all
	./sender & ./receiver