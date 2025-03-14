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
    libssl-dev \
    pkg-config \
    gcc

# Transfer files to container 

COPY src/ /src/

# Compilation

RUN gcc -o /bin/sender /src/sender.c /src/can_socket.c -Wall -lssl -lcrypto && \
    gcc -o /bin/receiver /src/receiver.c /src/can_socket.c -Wall -lssl -lcrypto

CMD ["/bin/bash"]
