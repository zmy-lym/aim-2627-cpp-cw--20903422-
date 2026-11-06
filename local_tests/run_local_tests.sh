#!/usr/bin/env bash
# Build and run every local boundary suite:
#
#     ./local_tests/run_local_tests.sh
#
# Each suite includes one src/main file with its `main` renamed, exactly like
# src/tests does, so the source under test is always the real submitted file.
set -u

here="$(cd "$(dirname "$0")" && pwd)"
root="$(cd "$here/.." && pwd)"
cxx="${CXX:-c++}"
failures=0

mkdir -p "$here/build"

for suite in "$here"/test_*_boundary.cpp; do
    name="$(basename "$suite" .cpp)"
    binary="$here/build/$name"
    echo "== $name =="
    # -fsanitize is optional locally; drop it if the toolchain lacks it.
    if ! "$cxx" -std=c++17 -O2 -Wall -Wextra -pedantic \
            -fsanitize=undefined,address -I "$root" \
            "$suite" -o "$binary"; then
        echo "   COMPILE ERROR"
        failures=$((failures + 1))
        continue
    fi
    if ! "$binary"; then
        failures=$((failures + 1))
    fi
done

echo
if [ "$failures" -ne 0 ]; then
    echo "$failures local suite(s) failed"
    exit 1
fi
echo "all local boundary suites passed"
