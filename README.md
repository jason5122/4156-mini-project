# COMS 4156 Mini Project

## Build

```bash
mkdir build && cd build
cmake ..
make
```

## Run

### Setting up the database

```bash
cd build
./mini_project setup
```

### Running the web server

```bash
cd build
./mini_project
```

## Test

```bash
cd build
./mini_project_test
```

### Code coverage using Gcov/Lcov

> [!NOTE]\
> Run tests at least once to generate the `.gcda` file before running `gcov`/`lcov`.

```bash
cd build/CMakeFiles/mini_project_test.dir/src
gcov *.cpp.gcno
lcov --filter range --ignore-errors inconsistent --capture --directory . -o coverage.info
lcov --filter range --ignore-errors inconsistent --remove coverage.info "/usr*" "include/" -o filtered_coverage.info
genhtml --filter range --ignore-errors inconsistent filtered_coverage.info --output-directory code_coverage
cd code_coverage
open index.html
```

## `clangd` integration

```bash
cd build
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 ..
mv compile_commands.json ../compile_commands.json
```

This should create a file called `compile_commands.json` in the root directory.
