FROM ubuntu:latest

# Environment variables
ENV SRC_DIR=/src
ENV BIN_DIR=/bin

# Obtain updated packages for the application
RUN apt update && apt install -y \
    build-essential \
    can-utils \
    iproute2 \
    iputils-ping

<<<<<<< Updated upstream
# Transfer files to container 

COPY ${SRC_DIR}/sender.c ${SRC_DIR}/sender.c
COPY ${SRC_DIR}/receiver.c ${SRC_DIR}/receiver.c

# Compilation

RUN gcc -o ${BIN_DIR}/sender ${SRC_DIR}/sender.c -Wall
RUN gcc -o ${BIN_DIR}/receiver ${SRC_DIR}/receiver.c -Wall
=======

COPY ${SRC_DIR} ${SRC_DIR}

# Compilation

RUN mkdir -p ${BIN_DIR}
RUN make -C ${SRC_DIR}
>>>>>>> Stashed changes

CMD ["/bin/bash"]