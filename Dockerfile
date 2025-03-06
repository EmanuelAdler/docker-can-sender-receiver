FROM ubuntu:latest

# definir variáveis de ambiente

ENV SRC_DIR=/src
ENV BIN_DIR=/bin

# obter pacotes necessários para compilação

RUN apt update && apt install -y \
    build-essential \
    can-utils \
    iproute2 \
    iputils-ping

# transferir arquivos da pasta para o container 

COPY ${SRC_DIR}/sender.c ${SRC_DIR}/sender.c
COPY ${SRC_DIR}/receiver.c ${SRC_DIR}/receiver.c

# compilação

RUN gcc -o ${BIN_DIR}/sender ${SRC_DIR}/sender.c -Wall
RUN gcc -o ${BIN_DIR}/receiver ${SRC_DIR}/receiver.c -Wall

CMD ["/bin/bash"]
