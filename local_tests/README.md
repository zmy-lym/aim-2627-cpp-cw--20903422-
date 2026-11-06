# Local boundary tests

These are extra boundary suites written while completing the assessment. They are
**not** part of the graded submission: the formal grader extracts only the region
between `BEGIN IMPLEMENTATION` and `END IMPLEMENTATION` in `src/main/A1_*.cpp`.

Every suite includes one `src/main` file with its `main` renamed, which is the
same technique `src/tests` uses. That way the code under test is always the real
submitted file rather than a copy that could drift away from it.

## Running

```bash
./local_tests/run_local_tests.sh
```

Or a single suite:

```bash
c++ -std=c++17 -O2 -Wall -Wextra -pedantic -I . \
    local_tests/test_q8_boundary.cpp -o /tmp/test_q8 && /tmp/test_q8
```

Compiled binaries go to `local_tests/build/`, which is ignored by Git.

## Naming

Files are named `test_<task>_boundary.cpp`, deliberately **not** `A1_<task>.cpp`.
The graded checker searches the repository for a file whose name equals the task
source name and refuses to grade when more than one file matches, so reusing the
`A1_*.cpp` names here would break grading.

## What each suite covers

| Suite | Focus |
| --- | --- |
| `test_q1_boundary.cpp` | Clamp before scaling, invalid maximum, battery thresholds 19/20/59/60, 10^12 magnitudes |
| `test_q2_boundary.cpp` | Complete-field matching, deletion at every position, append without a leading comma, length-changing edits |
| `test_q3_boundary.cpp` | Invalid window lengths, strict inequality, k==1 and k==n, 2*10^14 sums over 200000 frames |
| `test_q4_boundary.cpp` | The 10^6 range boundary, the accepted counter, malformed fields, counting invariant |
| `test_q5_boundary.cpp` | Whole-map validation before computing, diagonals excluded, 300x300 extremes |
| `test_q6_boundary.cpp` | Eligibility filters, three-level tie-breaking, squared distances beyond 32 bits |
| `test_q7_boundary.cpp` | Cool before firing, heat exactly 100, rejection consuming no resources |
| `test_q8_boundary.cpp` | Validate before deduplicating, ascending ids, clamping versus validation, count invariants |
| `test_b1_boundary.cpp` | Asymmetric int limits, output preserved on every failure, leading zeros |
| `test_b2_boundary.cpp` | Stability, duplicates, reversed bounds, nullptr with n==0 |
