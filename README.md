# Docker CAN Project  
This project consists of a simple application for communication between a sender and a receiver over a virtual CAN network.

## Requirements  
- Ubuntu or another Linux system compatible with SocketCAN
- Docker  
- Configured `vcan0` interface  

## CAN Interface Setup  
Give execution permission to the script:  
```sh
chmod +x setup_vcan.sh
```
Run the script to configure the interface:
```sh
sudo ./setup_vcan.sh
```

## Building and Running the Containers
Build and start the containers:
```sh
docker-compose up --build
```

Check the running containers:
```sh
docker ps
```

## Communication Test
Send a CAN frame:
```sh
docker exec ./bin/sender cansend vcan0 123#DEADBEEF
```

View the received messages:
```sh
docker exec .bin/receiver candump vcan0
```

To stop the containers:
```sh
docker-compose down
```

## Source code testing
Install the Cunit library and lcov for testing:
```sh
sudo apt-get install libcunit1 libcunit1-doc libcunit1-dev lcov
```
Then, in the test folder run:
```sh
make test
```
or
```sh
make coverage
```
Then, open another terminal in the same folder and run:
```sh
make send
```
to test the function that receives a CAN frame.