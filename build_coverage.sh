#!/bin/bash
set -e

# NOTE: Run tests at least once to generate the .gcda file.

# Get code coverage using LCOV.
cd build/CMakeFiles/mini_project_test.dir/src
gcov *.cpp.gcno
lcov --filter range --ignore-errors inconsistent --capture --directory . -o coverage.info
lcov --filter range --ignore-errors inconsistent --remove coverage.info "/usr*" "include/" -o filtered_coverage.info
genhtml --filter range --ignore-errors inconsistent filtered_coverage.info --output-directory code_coverage
cd code_coverage
open index.html
