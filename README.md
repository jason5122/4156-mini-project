# COMS 4156 Mini Project

## TL;DR: Build + Run All Tests + Code Coverage

```bash
mkdir build && cd build
cmake .. && make
./mini_project setup
./mini_project
```

In a separate terminal:

```bash
cd build
./mini_project_test
./mini_project_integration_test

cd ..
./build_coverage.sh
```

## Code Coverage Output

![Code Coverage](res/coverage.webp "Code Coverage")

## Development Only: `clangd` integration

```bash
cd build
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 ..
mv compile_commands.json ../compile_commands.json
```

This should create a file called `compile_commands.json` in the root directory, which `clangd` can detect.
