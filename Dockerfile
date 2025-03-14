FROM ubuntu:latest

# Environment variables

ENV SRC_DIR=/src
ENV BIN_DIR=/bin

# Obtain updated packages for the application

RUN apt update && apt install -y \
    build-essential \
    can-utils \
    iproute2 \
    iputils-ping \
    libssl-dev 

# Transfer files to container 

COPY ${SRC_DIR}/sender.c ${SRC_DIR}/sender.c
COPY ${SRC_DIR}/receiver.c ${SRC_DIR}/receiver.c
COPY ${SRC_DIR}/can_socket.c ${SRC_DIR}/can_socket.c
COPY ${SRC_DIR}/can_socket.h ${SRC_DIR}/can_socket.h

# Compilation

RUN gcc -o ${BIN_DIR}/sender ${SRC_DIR}/sender.c ${SRC_DIR}/can_socket.c -Wall -lssl -lcrypto
RUN gcc -o ${BIN_DIR}/receiver ${SRC_DIR}/receiver.c ${SRC_DIR}/can_socket.c -Wall

CMD ["/bin/bash"]
