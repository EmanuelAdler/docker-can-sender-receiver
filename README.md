# Docker CAN Project  
This project consists of a simple application for communication between a sender and a receiver over a virtual CAN network.

[![Build and Push Docker Images](https://github.com/EmanuelAdler/docker-can-sender-receiver/actions/workflows/docker-build.yml/badge.svg)](https://github.com/EmanuelAdler/docker-can-sender-receiver/actions/workflows/docker-build.yml)

## Requirements  
- Ubuntu or another Linux system compatible with SocketCAN
- Docker  

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

Our project utilizes GitHub Actions to automate various aspects of development and deployment. Below is an overview of each workflow:

### 1. Label Triage Workflow

**Purpose:** Automatically labels new issues with "triage" to streamline issue management.

**Workflow File:** `.github/workflows/label-triage.yml`

**Trigger:** Activates when a new issue is opened.

**Key Steps:**
- **Add "triage" Label:** Uses the `actions/github-script` action to add the "triage" label to new issues.

### 2. Linting Workflow

**Purpose:** Ensures code quality by performing static analysis on project files using `Clang-Tidy`.

**Workflow File:** `.github/workflows/linting.yml`

**Triggers:** Runs on every push and pull request.

**Key Steps:**
- **Checkout Code:** Retrieves the latest code from the repository.
- **Install Clang-Tidy:** Installs the `Clang-Tidy` tool on the runner.
- **Run Clang-Tidy:** Executes `Clang-Tidy` with MISRA-C compliance settings to analyze the C source files. The workflow is configured to fail if any issues are detected (`--error-exitcode=1`).

### 3. Docker Build and Push Workflow

**Purpose:** Automates the building and pushing of Docker images for the project.

**Workflow File:** `.github/workflows/docker-build.yml`

**Triggers:**
- Pushes to the `main` and `develop` branches.
- Creation of version tags matching the pattern `v*.*.*`.

**Key Steps:**
- **Checkout Code:** Retrieves the latest code from the repository.
- **Set up Docker Buildx:** Initializes Docker Buildx for advanced build capabilities.
- **Log in to Docker Hub:** Authenticates to Docker Hub using credentials stored in repository secrets (`DOCKERHUB_USERNAME` and `DOCKERHUB_TOKEN`).
- **Build and Push Docker Image:** Builds the Docker image and pushes it to Docker Hub with appropriate tags:
  - `latest` for the `main` and `develop` branches.
  - The specific Git reference name (e.g., branch name or tag).

### 4. Automated Testing Workflow

**Purpose:** Runs unit and integration tests to validate CAN message transmission.

**Workflow File:** `.github/workflows/auto-tests.yml`

**Triggers:**
- Runs on every pull request to the `develop` branch.

**Key Steps:**
- **Checkout Code:** Retrieves the latest code from the repository.
- **Install Dependencies:** Installs necessary packages, including `can-utils`, `libcunit1-dev`, `lcov`, and required kernel modules.
- **Set Up CAN Interface:** Executes `setup_vcan.sh` to configure the virtual CAN interface.
- **Run Tests:** Executes `make test` to compile and run unit and integration tests.

## Communication Test
Send a message via CAN:
```sh
echo "message" | docker exec -i sender sh -c 'cat > /tmp/can_pipe'
```

To stop the containers:
```sh
docker-compose down
```

## Source code testing
Install the required libraries for testing:
```sh
sudo apt-get install libssl-dev libcunit1 libcunit1-doc libcunit1-dev lcov
```
Then, in the test folder run:
```sh
make test
```
or
```sh
make coverage
```