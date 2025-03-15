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
    make

# Transfer files to container
COPY ${SRC_DIR} ${SRC_DIR}

# Ensure bin directory exists
RUN mkdir -p ${BIN_DIR}

# Compile using Makefile
RUN make -C ${SRC_DIR}

CMD ["/bin/bash"]