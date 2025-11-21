# Testing Yaqeen

Complete guide for testing Yaqeen functionality.

## Quick Start

```bash
cd /home/omen/yaqeen
mkdir build && cd build
cmake ..
make -j$(nproc)
```

## 1. Build the Project

First, let's build Yaqeen:

```bash
cd /home/omen/yaqeen
mkdir -p build
cd build

# Configure
cmake -DCMAKE_BUILD_TYPE=Debug ..

# Build (parallel compilation)
make -j$(nproc)
```

Expected output:
```
-- Fetching dependencies...
-- Build files have been written to: /home/omen/yaqeen/build
[ 10%] Building CXX object ...
[100%] Built target yaqeen
```

## 2. Run Unit Tests

Run the test suite:

```bash
# From build directory
ctest --output-on-failure

# Or run tests directly
./yaqeen_tests
```

Expected output:
```
Test project /home/omen/yaqeen/build
    Start 1: yaqeen_tests
1/1 Test #1: yaqeen_tests .....................   Passed    0.12 sec

100% tests passed, 0 tests failed
```

## 3. Test CLI Commands

### Test Help Command

```bash
./yaqeen --help
```

Expected: Display help information and logo.

### Test Version

```bash
./yaqeen --version
```

Expected: Display version information.

### Test List Templates

```bash
./yaqeen list
```

Expected: Display all templates grouped by category.

### Test Show Template

```bash
./yaqeen show react-typescript
```

Expected: Display detailed information about the React TypeScript template.

## 4. Test Project Creation

### Test 4.1: Create from Template

```bash
# Create a test project
./yaqeen create --template react-typescript --name test-react-app

# Check if it was created
ls -la test-react-app/
```

Expected: Project structure should be created with all directories and files.

### Test 4.2: Dry Run

```bash
./yaqeen create --template laravel --name test-laravel --dry-run
```

Expected: Show what would be created without actually creating files.

### Test 4.3: Verbose Output

```bash
./yaqeen create --template django --name test-django --verbose
```

Expected: Detailed output showing each file being created.

### Test 4.4: Custom Output Directory

```bash
./yaqeen create --template nextjs --name test-nextjs --output /tmp/yaqeen-test
ls -la /tmp/yaqeen-test/
```

Expected: Project created in specified directory.

## 5. Test Markdown Parsing

### Test 5.1: Create Markdown File

Create a test markdown file:

```bash
cd /home/omen/yaqeen/build

cat > test-structure.md << 'EOF'
# My Test Project

```
my-project/
├── src/
│   ├── main.cpp
│   ├── core/
│   │   ├── engine.cpp
│   │   └── engine.hpp
│   └── utils/
│       └── helpers.cpp
├── include/
│   └── myproject/
│       └── api.hpp
├── tests/
│   ├── test_main.cpp
│   └── test_core.cpp
├── CMakeLists.txt
├── README.md
└── .gitignore
```
EOF
```

### Test 5.2: Generate from Markdown

```bash
./yaqeen init test-structure.md

# Check results
ls -la my-project/
tree my-project/  # if tree is installed
```

Expected: Project structure matching the markdown should be created.

## 6. Test Different Templates

### Web Templates

```bash
./yaqeen create -t vue3 -n test-vue --dry-run
./yaqeen create -t nextjs -n test-next --dry-run
./yaqeen create -t angular -n test-angular --dry-run
```

### Backend Templates

```bash
./yaqeen create -t express -n test-express --dry-run
./yaqeen create -t fastapi -n test-fastapi --dry-run
./yaqeen create -t go-gin -n test-go --dry-run
```

### Architecture Patterns

```bash
./yaqeen create -t clean-architecture -n test-clean --dry-run
./yaqeen create -t microservices -n test-micro --dry-run
```

## 7. Test Error Handling

### Test Invalid Template

```bash
./yaqeen create --template nonexistent --name test
```

Expected: Error message about template not found.

### Test Missing Arguments

```bash
./yaqeen create --template react-typescript
```

Expected: Error about missing required argument.

### Test Invalid Markdown

Create invalid markdown:

```bash
cat > invalid.md << 'EOF'
This is not a valid structure
EOF

./yaqeen init invalid.md
```

Expected: Error about invalid markdown format.

## 8. Performance Testing

### Test Large Project Generation

```bash
# Test with a complex template
time ./yaqeen create --template microservices --name perf-test

# Clean up
rm -rf perf-test
```

Expected: Should complete in under 1 second for most templates.

### Test Multiple Projects

```bash
#!/bin/bash
# Create multiple projects rapidly
for i in {1..10}; do
    ./yaqeen create -t react-typescript -n "test-$i" -o "/tmp/test-$i" --dry-run
done
```

Expected: Fast execution without crashes.

## 9. Memory Leak Testing

If Valgrind is installed:

```bash
# Install valgrind (if needed)
sudo apt install valgrind  # Ubuntu/Debian

# Test for memory leaks
valgrind --leak-check=full --show-leak-kinds=all ./yaqeen create -t react-typescript -n test --dry-run
```

Expected: No memory leaks reported.

## 10. Integration Testing Script

Create a comprehensive test script:

```bash
cat > /home/omen/yaqeen/test-all.sh << 'EOF'
#!/bin/bash

set -e  # Exit on error

echo "======================================"
echo "Yaqeen Integration Test Suite"
echo "======================================"
echo ""

YAQEEN="./yaqeen"
TEST_DIR="/tmp/yaqeen-test-$$"
mkdir -p "$TEST_DIR"

cd "$(dirname "$0")/build"

# Test 1: Help
echo "[TEST 1] Testing help command..."
$YAQEEN --help > /dev/null
echo "✓ Help command works"
echo ""

# Test 2: List templates
echo "[TEST 2] Testing list command..."
$YAQEEN list > /dev/null
echo "✓ List command works"
echo ""

# Test 3: Show template
echo "[TEST 3] Testing show command..."
$YAQEEN show react-typescript > /dev/null
echo "✓ Show command works"
echo ""

# Test 4: Create from template (dry-run)
echo "[TEST 4] Testing create with dry-run..."
$YAQEEN create -t react-typescript -n test-app --dry-run > /dev/null
echo "✓ Dry-run works"
echo ""

# Test 5: Create actual project
echo "[TEST 5] Testing actual project creation..."
$YAQEEN create -t react-typescript -n "$TEST_DIR/test-app" > /dev/null
if [ -d "$TEST_DIR/test-app" ]; then
    echo "✓ Project created successfully"
else
    echo "✗ Project creation failed"
    exit 1
fi
echo ""

# Test 6: Create from markdown
echo "[TEST 6] Testing markdown parsing..."
cat > "$TEST_DIR/test.md" << 'MDEOF'
```
test-project/
├── src/
│   └── main.cpp
└── README.md
```
MDEOF

$YAQEEN init "$TEST_DIR/test.md" -o "$TEST_DIR/md-project" > /dev/null
if [ -d "$TEST_DIR/md-project" ]; then
    echo "✓ Markdown parsing works"
else
    echo "✗ Markdown parsing failed"
    exit 1
fi
echo ""

# Test 7: Multiple templates
echo "[TEST 7] Testing multiple templates..."
TEMPLATES=("vue3" "express" "flutter" "clean-architecture" "go-project")
for tmpl in "${TEMPLATES[@]}"; do
    $YAQEEN create -t "$tmpl" -n "test-$tmpl" --dry-run > /dev/null
    echo "  ✓ Template $tmpl works"
done
echo ""

# Cleanup
echo "Cleaning up test directory..."
rm -rf "$TEST_DIR"

echo "======================================"
echo "All tests passed! ✓"
echo "======================================"
EOF

chmod +x /home/omen/yaqeen/test-all.sh
```

Run the integration tests:

```bash
cd /home/omen/yaqeen
./test-all.sh
```

## 11. Visual Testing

Test the UI appearance:

```bash
# Test logo display
./yaqeen --help

# Test progress display
./yaqeen create -t laravel -n visual-test --verbose

# Test with different templates
./yaqeen create -t microservices -n ui-test
```

Expected: Beautiful Tokyo Night themed output with colored text, progress bars, and clean formatting.

## 12. Clean Up

After testing, clean up test projects:

```bash
# Remove test projects
rm -rf test-* my-project perf-test visual-test ui-test

# Remove temporary files
rm -rf /tmp/yaqeen-test*
```

## Troubleshooting

### Build Fails

```bash
# Clean and rebuild
cd /home/omen/yaqeen
rm -rf build
mkdir build && cd build
cmake ..
make -j$(nproc)
```

### Tests Fail

```bash
# Run tests with verbose output
ctest --verbose --output-on-failure
```

### Templates Not Found

```bash
# Check templates directory
ls -la /home/omen/yaqeen/templates/

# Run from build directory
cd /home/omen/yaqeen/build
./yaqeen list
```

### Permission Issues

```bash
# Make sure yaqeen is executable
chmod +x ./yaqeen

# Test in a writable directory
cd /tmp
/home/omen/yaqeen/build/yaqeen create -t react-typescript -n test-app
```

## Success Criteria

✓ All unit tests pass
✓ All CLI commands work
✓ Projects are created correctly
✓ Markdown parsing works
✓ Templates load successfully
✓ No memory leaks
✓ Error handling works
✓ UI looks beautiful
✓ Performance is fast (<1s for most templates)

## Reporting Issues

If you find issues:

1. Check build output for errors
2. Run with `--verbose` flag
3. Check log files if `--log-file` is used
4. Report on GitHub with:
   - Command used
   - Expected behavior
   - Actual behavior
   - Error messages
   - System information

Happy testing!
