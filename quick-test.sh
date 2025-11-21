#!/bin/bash
# Quick test script for Yaqeen

set -e

echo "=========================================="
echo "Yaqeen Quick Test"
echo "=========================================="
echo ""

# Colors for output
GREEN='\033[0;32m'
RED='\033[0;31m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Check if we're in the right directory
if [ ! -f "CMakeLists.txt" ]; then
    echo -e "${RED}Error: Run this script from the yaqeen root directory${NC}"
    exit 1
fi

echo -e "${BLUE}Step 1: Building Yaqeen...${NC}"
mkdir -p build
cd build

cmake -DCMAKE_BUILD_TYPE=Release .. || {
    echo -e "${RED}CMake configuration failed${NC}"
    exit 1
}

make -j$(nproc) || {
    echo -e "${RED}Build failed${NC}"
    exit 1
}

echo -e "${GREEN}✓ Build successful${NC}"
echo ""

echo -e "${BLUE}Step 2: Running unit tests...${NC}"
if [ -f "yaqeen_tests" ]; then
    ctest --output-on-failure || {
        echo -e "${RED}Tests failed${NC}"
        exit 1
    }
    echo -e "${GREEN}✓ All tests passed${NC}"
else
    echo -e "${RED}Warning: Test binary not found, skipping tests${NC}"
fi
echo ""

echo -e "${BLUE}Step 3: Testing CLI commands...${NC}"

# Test help
echo "  Testing --help..."
./yaqeen --help > /dev/null 2>&1
echo -e "${GREEN}  ✓ Help works${NC}"

# Test list
echo "  Testing list command..."
./yaqeen list > /dev/null 2>&1
echo -e "${GREEN}  ✓ List works${NC}"

# Test show
echo "  Testing show command..."
./yaqeen show react-typescript > /dev/null 2>&1
echo -e "${GREEN}  ✓ Show works${NC}"

echo ""

echo -e "${BLUE}Step 4: Testing project creation (dry-run)...${NC}"

# Test with dry-run (doesn't create files)
./yaqeen create --template react-typescript --name test-app --dry-run > /dev/null 2>&1
echo -e "${GREEN}✓ Template generation works${NC}"
echo ""

echo -e "${BLUE}Step 5: Creating a real test project...${NC}"

# Create actual project in /tmp
TEST_DIR="/tmp/yaqeen-quick-test-$$"
./yaqeen create --template react-typescript --name "$TEST_DIR"

if [ -d "$TEST_DIR" ]; then
    echo -e "${GREEN}✓ Project created successfully at $TEST_DIR${NC}"
    echo ""
    echo "Project contents:"
    tree "$TEST_DIR" 2>/dev/null || ls -la "$TEST_DIR"
    echo ""

    # Cleanup
    read -p "Remove test project? (y/n) " -n 1 -r
    echo ""
    if [[ $REPLY =~ ^[Yy]$ ]]; then
        rm -rf "$TEST_DIR"
        echo "Test project removed"
    else
        echo "Test project kept at: $TEST_DIR"
    fi
else
    echo -e "${RED}✗ Project creation failed${NC}"
    exit 1
fi

echo ""
echo "=========================================="
echo -e "${GREEN}All tests passed! ✓${NC}"
echo "=========================================="
echo ""
echo "Yaqeen is ready to use!"
echo ""
echo "Try these commands:"
echo "  ./yaqeen list                                    # List all templates"
echo "  ./yaqeen show laravel                            # Show template details"
echo "  ./yaqeen create -t vue3 -n my-app                # Create a project"
echo "  ./yaqeen init structure.md                       # Create from markdown"
echo ""
echo "For more information, see TESTING.md and README.md"
