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
## CI/CD Pipeline

### Linting Workflow
- **Tool:** cppcheck is used to perform static analysis on `sender.c` and `receiver.c`.
- **Configuration:** We run cppcheck with `--enable=all --suppress=missingIncludeSystem --std=c99`.
- **Trigger:** This workflow runs on every push and pull request to ensure code quality.

### Docker Build & Deployment Workflow
- **Build Process:**  
  The Dockerfile compiles the CAN applications from `sender.c` and `receiver.c`.
- **Tagging:**  
  Images are tagged as `latest` on the `main`/`develop` branches and also tagged with the Git version (e.g., `v1.0.0`) when version tags are pushed.
- **Deployment:**  
  The built images are pushed to Docker Hub. Ensure your Docker Hub credentials are configured in GitHub Secrets (`DOCKERHUB_USERNAME` and `DOCKERHUB_TOKEN`).

### Virtual CAN Interface Setup
Before running the Docker containers locally, set up the virtual CAN interface by running:
```bash
sudo bash setup_vcan.sh
```

## Communication Test
Send a CAN frame:
```sh
docker exec sender cansend vcan0 123#DEADBEEF
```

View the received messages:
```sh
docker exec receiver candump vcan0
```

To stop the containers:
```sh
docker-compose down
```
