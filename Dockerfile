FROM ubuntu:latest

# definir variáveis de ambiente

ENV SRC_DIR=/src
#ENV BIN_DIR=./bin

RUN apt update && apt install -y \
    build-essential \
    can-utils \
    iproute2 \
    iputils-ping

COPY ${SRC_DIR}/sender.c sender.c
COPY ${SRC_DIR}/receiver.c receiver.c

RUN gcc -o sender sender.c -Wall
RUN gcc -o receiver receiver.c -Wall

CMD ["/bin/bash"]
