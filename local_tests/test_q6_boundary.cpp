// Local boundary tests for Q6. Not part of the submitted implementation:
// the graded region lives in src/main/A1_q6.cpp only.
#define main aim_local_q6_embedded_main
#include "../src/main/A1_q6.cpp"
#undef main

int main() {
    try {
        // Clause 2: a negative range admits nothing; empty input returns -1.
        CHECK(select_target({}, 5) == -1);
        CHECK(select_target({{1, 0, 0, 5, true}}, -1) == -1);

        // Clause 3: eligibility needs visible, hp>0 and squared distance within
        // the inclusive range boundary.
        CHECK(select_target({{1, 0, 0, 0, true}}, 1) == -1);
        CHECK(select_target({{1, 0, 0, 5, false}}, 10) == -1);
        CHECK(select_target({{1, 0, 0, -5, true}}, 10) == -1);
        CHECK(select_target({{1, 3, 4, 1, true}}, 5) == 1);
        CHECK(select_target({{1, 3, 4, 1, true}, {2, 0, 0, 2, true}}, 4) == 2);

        // Clause 4: lower hp first, then squared distance, then lower id.
        CHECK(select_target({{7, 3, 4, 20, true}, {2, 0, 5, 20, true}}, 5) == 2);
        CHECK(select_target({{7, 3, 4, 1, true}, {2, 0, 0, 2, true}}, 5) == 7);
        CHECK(select_target({{9, 0, 0, 3, true}, {1, 0, 0, 3, true}}, 0) == 1);

        // Clause 5: the id is returned, not the array index, and input order is
        // never the tie-breaker.
        CHECK(select_target({{8, 1, 0, 5, true}, {3, 0, 1, 5, true}}, 10) == 3);
        CHECK(select_target({{3, 0, 1, 5, true}, {8, 1, 0, 5, true}}, 10) == 3);

        // Clause 6: squares must use a wide integer. At 10^6 the sum reaches
        // 2*10^12, which overflows 32 bits, so the far target must be rejected.
        CHECK(select_target({{1, 1000000, 1000000, 5, true}}, 1000000) == -1);
        CHECK(select_target({{1, 1000000, 0, 5, true}}, 1000000) == 1);
        CHECK(select_target({{5, -1000000, 0, 5, true}}, 1000000) == 5);

        // A zero range admits only the origin.
        CHECK(select_target({{1, 0, 0, 5, true}}, 0) == 1);
        CHECK(select_target({{1, 0, 1, 5, true}}, 0) == -1);

        std::cout << "Q6 local boundary tests: PASS\n";
    } catch (const std::exception& e) {
        std::cerr << "Q6 local boundary tests: FAIL: " << e.what() << "\n";
        return 1;
    }
    return 0;
}
