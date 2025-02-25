FROM ubuntu:latest

RUN apt update && apt install -y \
    build-essential \
    can-utils \
    iproute2 \
    iputils-ping

COPY sender.c /sender.c
COPY receiver.c /receiver.c

RUN gcc -o sender sender.c -Wall
RUN gcc -o receiver receiver.c -Wall

CMD ["/bin/bash"]
