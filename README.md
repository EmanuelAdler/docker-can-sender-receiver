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

Our project utilizes GitHub Actions to automate various aspects of development and deployment. Below is an overview of each workflow:

### 1. Label Triage Workflow

**Purpose:** Automatically labels new issues with "triage" to streamline issue management.

**Workflow File:** `.github/workflows/label-triage.yml`

**Trigger:** Activates when a new issue is opened.

**Key Steps:**
- **Add "triage" Label:** Uses the `actions/github-script` action to add the "triage" label to new issues.

**Note:** Ensure that the `GH_PAT` (GitHub Personal Access Token) secret is configured in your repository settings to authorize labeling.

### 2. Linting Workflow

**Purpose:** Ensures code quality by performing static analysis on `sender.c` and `receiver.c` using `cppcheck`.

**Workflow File:** `.github/workflows/linting.yml`

**Triggers:** Runs on every push and pull request.

**Key Steps:**
- **Checkout Code:** Retrieves the latest code from the repository.
- **Install cppcheck:** Installs the `cppcheck` tool on the runner.
- **Run cppcheck:** Executes `cppcheck` with MISRA-C compliance settings to analyze the C source files. The workflow is configured to fail if any issues are detected (`--error-exitcode=1`).

**Note:** Customize the `cppcheck` command as needed, especially if you have a specific MISRA-C rule file.

### 3. Docker Build and Push Workflow

**Purpose:** Automates the building and pushing of Docker images for the project.

**Workflow File:** `.github/workflows/docker-build.yml`

**Triggers:**
- Pushes to the `main` and `develop` branches.
- Creation of version tags matching the pattern `v*.*.*`.

**Key Steps:**
- **Checkout Code:** Retrieves the latest code from the repository.
- **Set up QEMU:** Prepares the environment for cross-platform Docker builds.
- **Set up Docker Buildx:** Initializes Docker Buildx for advanced build capabilities.
- **Log in to Docker Hub:** Authenticates to Docker Hub using credentials stored in repository secrets (`DOCKERHUB_USERNAME` and `DOCKERHUB_TOKEN`).
- **Build and Push Docker Image:** Builds the Docker image and pushes it to Docker Hub with appropriate tags:
  - `latest` for the `main` and `develop` branches.
  - The specific Git reference name (e.g., branch name or tag).

**Note:** Replace `yourdockerhubusername/yourimagename` with your actual Docker Hub username and desired image name.

## Troubleshooting Tips

- **Permissions Issues:** Ensure that all necessary secrets (e.g., `GH_PAT`, `DOCKERHUB_USERNAME`, `DOCKERHUB_TOKEN`) are correctly configured in your repository settings.
- **Workflow Failures:** Review the logs provided by GitHub Actions to identify and address any errors. Common issues include syntax errors in workflow files or missing dependencies.
- **Docker Build Problems:** Verify that your `Dockerfile` and `docker-compose.yml` are correctly configured and that all necessary files are included in the build context.

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