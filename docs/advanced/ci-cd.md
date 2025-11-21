# CI/CD Integration

Integrate Yaqeen into your Continuous Integration and Continuous Deployment pipelines to automate project scaffolding, testing, and deployment workflows.

## Overview

Yaqeen integrates seamlessly with popular CI/CD platforms:
- **GitHub Actions**
- **GitLab CI**
- **Jenkins**
- **CircleCI**
- **Travis CI**
- **Azure Pipelines**
- **Bitbucket Pipelines**

## GitHub Actions

### Basic Workflow

`.github/workflows/scaffold.yml`:
```yaml
name: Scaffold Project

on:
  workflow_dispatch:
    inputs:
      template:
        description: 'Template name'
        required: true
        default: 'react-typescript'
      project_name:
        description: 'Project name'
        required: true

jobs:
  scaffold:
    runs-on: ubuntu-latest

    steps:
      - name: Checkout
        uses: actions/checkout@v4

      - name: Install Yaqeen
        run: |
          wget https://github.com/yourusername/yaqeen/releases/latest/download/yaqeen-linux-x64
          chmod +x yaqeen-linux-x64
          sudo mv yaqeen-linux-x64 /usr/local/bin/yaqeen

      - name: Create Project
        run: |
          yaqeen create \
            --template ${{ github.event.inputs.template }} \
            --name ${{ github.event.inputs.project_name }} \
            --output ./generated \
            --verbose

      - name: Upload Artifact
        uses: actions/upload-artifact@v3
        with:
          name: generated-project
          path: ./generated/${{ github.event.inputs.project_name }}
```

### Matrix Strategy

Generate multiple projects in parallel:

```yaml
name: Generate Multiple Projects

on: push

jobs:
  generate:
    runs-on: ubuntu-latest
    strategy:
      matrix:
        template:
          - react-typescript
          - vue3
          - nextjs
        include:
          - template: react-typescript
            test_cmd: npm run test
          - template: vue3
            test_cmd: npm run test:unit
          - template: nextjs
            test_cmd: npm run test

    steps:
      - name: Install Yaqeen
        run: |
          wget https://github.com/yourusername/yaqeen/releases/latest/download/yaqeen-linux-x64
          chmod +x yaqeen-linux-x64
          sudo mv yaqeen-linux-x64 /usr/local/bin/yaqeen

      - name: Generate Project
        run: |
          yaqeen create \
            --template ${{ matrix.template }} \
            --name test-${{ matrix.template }} \
            --verbose

      - name: Install Dependencies
        working-directory: ./test-${{ matrix.template }}
        run: npm install

      - name: Run Tests
        working-directory: ./test-${{ matrix.template }}
        run: ${{ matrix.test_cmd }}
```

### Template Testing Workflow

Test templates on every commit:

```yaml
name: Test Templates

on: [push, pull_request]

jobs:
  test:
    runs-on: ubuntu-latest

    steps:
      - name: Checkout
        uses: actions/checkout@v4

      - name: Build Yaqeen
        run: |
          mkdir build && cd build
          cmake -DCMAKE_BUILD_TYPE=Release ..
          make -j$(nproc)
          sudo make install

      - name: Test All Templates
        run: |
          #!/bin/bash
          set -e

          templates=$(yaqeen list --format json | jq -r '.[].name')

          for template in $templates; do
            echo "Testing template: $template"
            yaqeen create --template $template --name test-$template --dry-run
          done

      - name: Generate Test Projects
        run: |
          #!/bin/bash
          set -e

          # Test a subset of templates
          templates=("react-typescript" "django" "go-gin")

          for template in "${templates[@]}"; do
            echo "Generating $template..."
            yaqeen create --template $template --name test-$template

            # Verify structure
            test -d test-$template || { echo "Failed to create test-$template"; exit 1; }
          done
```

### Deployment Workflow

Generate and deploy projects:

```yaml
name: Generate and Deploy

on:
  push:
    branches: [main]

jobs:
  deploy:
    runs-on: ubuntu-latest

    steps:
      - name: Checkout
        uses: actions/checkout@v4

      - name: Install Yaqeen
        run: |
          wget https://github.com/yourusername/yaqeen/releases/latest/download/yaqeen-linux-x64
          chmod +x yaqeen-linux-x64
          sudo mv yaqeen-linux-x64 /usr/local/bin/yaqeen

      - name: Generate Microservices
        run: |
          yaqeen create --template microservices --name platform

      - name: Build Docker Images
        run: |
          cd platform
          docker-compose build

      - name: Push to Registry
        env:
          DOCKER_USERNAME: ${{ secrets.DOCKER_USERNAME }}
          DOCKER_PASSWORD: ${{ secrets.DOCKER_PASSWORD }}
        run: |
          echo "$DOCKER_PASSWORD" | docker login -u "$DOCKER_USERNAME" --password-stdin
          docker-compose push

      - name: Deploy to Kubernetes
        run: |
          kubectl apply -f platform/infrastructure/k8s/
```

## GitLab CI

### Basic Pipeline

`.gitlab-ci.yml`:
```yaml
stages:
  - install
  - scaffold
  - test
  - deploy

variables:
  YAQEEN_VERSION: "1.0.0"

install_yaqeen:
  stage: install
  script:
    - wget https://github.com/yourusername/yaqeen/releases/download/v${YAQEEN_VERSION}/yaqeen-linux-x64
    - chmod +x yaqeen-linux-x64
    - mv yaqeen-linux-x64 /usr/local/bin/yaqeen
    - yaqeen --version
  artifacts:
    paths:
      - /usr/local/bin/yaqeen
    expire_in: 1 hour

scaffold_project:
  stage: scaffold
  dependencies:
    - install_yaqeen
  script:
    - yaqeen create --template $TEMPLATE --name $PROJECT_NAME --verbose
  artifacts:
    paths:
      - $PROJECT_NAME/
    expire_in: 1 day
  only:
    - main

test_templates:
  stage: test
  script:
    - |
      templates="react-typescript vue3 nextjs django fastapi"
      for template in $templates; do
        echo "Testing $template"
        yaqeen create --template $template --name test-$template --dry-run
      done
  only:
    - merge_requests

deploy:
  stage: deploy
  script:
    - cd $PROJECT_NAME
    - docker build -t myapp:$CI_COMMIT_SHA .
    - docker push myapp:$CI_COMMIT_SHA
  only:
    - main
```

### Dynamic Child Pipelines

```yaml
generate_configs:
  stage: prepare
  script:
    - |
      yaqeen list --format json > templates.json

      # Generate child pipeline config
      python3 << EOF
      import json

      with open('templates.json') as f:
          templates = json.load(f)

      config = {'stages': ['test']}
      for tmpl in templates:
          job_name = f"test_{tmpl['name']}"
          config[job_name] = {
              'stage': 'test',
              'script': [
                  f"yaqeen create --template {tmpl['name']} --name test --dry-run"
              ]
          }

      with open('child-pipeline.yml', 'w') as f:
          import yaml
          yaml.dump(config, f)
      EOF
  artifacts:
    paths:
      - child-pipeline.yml

trigger_tests:
  stage: test
  trigger:
    include:
      - artifact: child-pipeline.yml
        job: generate_configs
    strategy: depend
```

## Jenkins

### Declarative Pipeline

`Jenkinsfile`:
```groovy
pipeline {
    agent any

    parameters {
        choice(name: 'TEMPLATE', choices: ['react-typescript', 'vue3', 'nextjs', 'django'], description: 'Template to use')
        string(name: 'PROJECT_NAME', defaultValue: 'my-project', description: 'Project name')
    }

    stages {
        stage('Install Yaqeen') {
            steps {
                sh '''
                    wget https://github.com/yourusername/yaqeen/releases/latest/download/yaqeen-linux-x64
                    chmod +x yaqeen-linux-x64
                    sudo mv yaqeen-linux-x64 /usr/local/bin/yaqeen
                '''
            }
        }

        stage('Generate Project') {
            steps {
                sh '''
                    yaqeen create \
                        --template ${TEMPLATE} \
                        --name ${PROJECT_NAME} \
                        --output ./generated \
                        --verbose
                '''
            }
        }

        stage('Test') {
            steps {
                dir("generated/${PROJECT_NAME}") {
                    sh '''
                        if [ -f package.json ]; then
                            npm install
                            npm test
                        elif [ -f requirements.txt ]; then
                            pip install -r requirements.txt
                            pytest
                        fi
                    '''
                }
            }
        }

        stage('Archive') {
            steps {
                archiveArtifacts artifacts: 'generated/**', fingerprint: true
            }
        }
    }

    post {
        always {
            cleanWs()
        }
        success {
            echo 'Project scaffolded successfully!'
        }
        failure {
            echo 'Scaffolding failed!'
        }
    }
}
```

### Multibranch Pipeline

```groovy
def templates = ['react-typescript', 'vue3', 'nextjs']

pipeline {
    agent any

    stages {
        stage('Parallel Generation') {
            steps {
                script {
                    def parallelStages = [:]

                    templates.each { template ->
                        parallelStages[template] = {
                            node {
                                stage("Generate ${template}") {
                                    sh "yaqeen create --template ${template} --name test-${template}"
                                }
                                stage("Test ${template}") {
                                    dir("test-${template}") {
                                        sh "npm install && npm test"
                                    }
                                }
                            }
                        }
                    }

                    parallel parallelStages
                }
            }
        }
    }
}
```

## CircleCI

`.circleci/config.yml`:
```yaml
version: 2.1

executors:
  yaqeen-executor:
    docker:
      - image: cimg/base:stable
    working_directory: ~/project

commands:
  install-yaqeen:
    steps:
      - run:
          name: Install Yaqeen
          command: |
            wget https://github.com/yourusername/yaqeen/releases/latest/download/yaqeen-linux-x64
            chmod +x yaqeen-linux-x64
            sudo mv yaqeen-linux-x64 /usr/local/bin/yaqeen
            yaqeen --version

jobs:
  scaffold:
    executor: yaqeen-executor
    parameters:
      template:
        type: string
      project_name:
        type: string
    steps:
      - checkout
      - install-yaqeen
      - run:
          name: Generate Project
          command: |
            yaqeen create \
              --template << parameters.template >> \
              --name << parameters.project_name >> \
              --verbose
      - persist_to_workspace:
          root: .
          paths:
            - << parameters.project_name >>

  test:
    executor: yaqeen-executor
    parameters:
      project_name:
        type: string
    steps:
      - attach_workspace:
          at: .
      - run:
          name: Test Project
          command: |
            cd << parameters.project_name >>
            if [ -f package.json ]; then
              npm install
              npm test
            fi

workflows:
  version: 2
  scaffold-and-test:
    jobs:
      - scaffold:
          name: scaffold-react
          template: react-typescript
          project_name: test-react
      - test:
          name: test-react
          project_name: test-react
          requires:
            - scaffold-react

      - scaffold:
          name: scaffold-vue
          template: vue3
          project_name: test-vue
      - test:
          name: test-vue
          project_name: test-vue
          requires:
            - scaffold-vue
```

## Docker Integration

### Dockerfile for CI

```dockerfile
FROM ubuntu:22.04

# Install dependencies
RUN apt-get update && apt-get install -y \
    wget \
    curl \
    git \
    build-essential \
    && rm -rf /var/lib/apt/lists/*

# Install Yaqeen
RUN wget https://github.com/yourusername/yaqeen/releases/latest/download/yaqeen-linux-x64 \
    && chmod +x yaqeen-linux-x64 \
    && mv yaqeen-linux-x64 /usr/local/bin/yaqeen

# Verify installation
RUN yaqeen --version

WORKDIR /workspace

ENTRYPOINT ["yaqeen"]
```

Usage:
```bash
# Build image
docker build -t yaqeen:latest .

# Use in CI
docker run --rm -v $(pwd):/workspace yaqeen:latest create --template react --name myapp
```

### Docker Compose for Multi-Service Generation

`docker-compose.yml`:
```yaml
version: '3.8'

services:
  generator:
    image: yaqeen:latest
    volumes:
      - ./output:/workspace
    command: create --template microservices --name platform

  frontend:
    image: node:18
    volumes:
      - ./output/platform/frontend:/app
    working_dir: /app
    command: npm install
    depends_on:
      - generator

  backend:
    image: node:18
    volumes:
      - ./output/platform/backend:/app
    working_dir: /app
    command: npm install
    depends_on:
      - generator
```

## Automation Scripts

### Bash Script for Automated Scaffolding

`scaffold.sh`:
```bash
#!/bin/bash
set -e

# Configuration
TEMPLATE="${1:-react-typescript}"
PROJECT_NAME="${2:-my-project}"
OUTPUT_DIR="${3:-./projects}"

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

echo -e "${GREEN}Starting project scaffolding...${NC}"

# Check if yaqeen is installed
if ! command -v yaqeen &> /dev/null; then
    echo -e "${RED}Yaqeen is not installed. Installing...${NC}"
    wget https://github.com/yourusername/yaqeen/releases/latest/download/yaqeen-linux-x64
    chmod +x yaqeen-linux-x64
    sudo mv yaqeen-linux-x64 /usr/local/bin/yaqeen
fi

# Verify template exists
if ! yaqeen list | grep -q "$TEMPLATE"; then
    echo -e "${RED}Template '$TEMPLATE' not found${NC}"
    echo "Available templates:"
    yaqeen list
    exit 1
fi

# Create project
echo -e "${YELLOW}Creating project: $PROJECT_NAME${NC}"
echo -e "${YELLOW}Template: $TEMPLATE${NC}"
echo -e "${YELLOW}Output: $OUTPUT_DIR${NC}"

yaqeen create \
    --template "$TEMPLATE" \
    --name "$PROJECT_NAME" \
    --output "$OUTPUT_DIR" \
    --verbose

# Post-generation steps
cd "$OUTPUT_DIR/$PROJECT_NAME"

if [ -f package.json ]; then
    echo -e "${YELLOW}Installing npm dependencies...${NC}"
    npm install
fi

if [ -f requirements.txt ]; then
    echo -e "${YELLOW}Installing Python dependencies...${NC}"
    pip install -r requirements.txt
fi

# Initialize git
if [ ! -d .git ]; then
    echo -e "${YELLOW}Initializing git repository...${NC}"
    git init
    git add .
    git commit -m "chore: initial commit from yaqeen"
fi

echo -e "${GREEN}✓ Project created successfully!${NC}"
echo -e "${GREEN}Location: $OUTPUT_DIR/$PROJECT_NAME${NC}"
```

### Python Script for Batch Generation

`batch_generate.py`:
```python
#!/usr/bin/env python3

import subprocess
import sys
import json
from pathlib import Path

def run_command(cmd):
    """Run shell command and return output."""
    result = subprocess.run(cmd, shell=True, capture_output=True, text=True)
    if result.returncode != 0:
        print(f"Error: {result.stderr}", file=sys.stderr)
        sys.exit(1)
    return result.stdout

def get_templates():
    """Get list of available templates."""
    output = run_command("yaqeen list --format json")
    return json.loads(output)

def generate_project(template, name, output_dir):
    """Generate a project from template."""
    print(f"Generating {name} from {template}...")
    cmd = f"yaqeen create --template {template} --name {name} --output {output_dir}"
    run_command(cmd)
    print(f"✓ {name} created successfully")

def main():
    # Configuration
    projects = [
        {"template": "react-typescript", "name": "web-app"},
        {"template": "fastapi", "name": "api-service"},
        {"template": "flutter", "name": "mobile-app"},
    ]

    output_dir = "./generated"
    Path(output_dir).mkdir(exist_ok=True)

    # Generate all projects
    for project in projects:
        generate_project(
            project["template"],
            project["name"],
            output_dir
        )

    print("\n✓ All projects generated successfully!")

if __name__ == "__main__":
    main()
```

## Best Practices

### 1. Cache Yaqeen Installation

```yaml
# GitHub Actions
- name: Cache Yaqeen
  uses: actions/cache@v3
  with:
    path: /usr/local/bin/yaqeen
    key: yaqeen-${{ runner.os }}-v1.0.0
```

### 2. Use Specific Versions

```bash
# Pin to specific version
YAQEEN_VERSION="1.0.0"
wget https://github.com/yourusername/yaqeen/releases/download/v${YAQEEN_VERSION}/yaqeen-linux-x64
```

### 3. Validate Templates Before Use

```bash
# Dry run first
yaqeen create --template $TEMPLATE --name test --dry-run
```

### 4. Log All Operations

```bash
yaqeen create --template $TEMPLATE --name $NAME --log-file ci-scaffold.log --verbose
```

### 5. Use Artifacts

Store generated projects as artifacts for later stages.

### 6. Run in Parallel

Generate multiple projects concurrently for faster CI.

## Troubleshooting

### Permission Denied

```bash
# Ensure execute permissions
chmod +x yaqeen-linux-x64
```

### Template Not Found

```bash
# Verify template availability
yaqeen list | grep $TEMPLATE
```

### Insufficient Disk Space

```bash
# Check disk space
df -h
# Clean up before running
yaqeen clean
```

## Summary

Key takeaways:
- ✅ Yaqeen integrates with all major CI/CD platforms
- ✅ Use Docker for consistent environments
- ✅ Cache installations for faster builds
- ✅ Run template tests in CI
- ✅ Automate project generation in pipelines
- ✅ Use matrix strategies for parallel execution

Next steps:
- Set up CI/CD pipeline
- Create automation scripts
- Test templates automatically
- Deploy generated projects
