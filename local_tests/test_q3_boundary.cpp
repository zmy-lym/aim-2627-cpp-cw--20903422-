// Local boundary tests for Q3. Not part of the submitted implementation:
// the graded region lives in src/main/A1_q3.cpp only.
#define main aim_local_q3_embedded_main
#include "../src/main/A1_q3.cpp"
#undef main

int main() {
    try {
        // Clause 2: invalid window lengths return an empty sequence without
        // forming partial windows.
        CHECK(alarms({1}, 0, 0).empty());
        CHECK(alarms({1}, -1, 0).empty());
        CHECK(alarms({1, 2}, 3, 0).empty());
        CHECK(alarms({}, 1, 0).empty());

        // Clause 4: strictly greater than the threshold; equality is silent.
        CHECK((alarms({2, 2}, 2, 4).empty()));
        CHECK((alarms({2, 2}, 2, 3) == std::vector<int>{1}));

        // Clause 1: negative readings are valid data, not errors.
        CHECK((alarms({-4, -3, -2}, 2, -6) == std::vector<int>{2}));

        // Clause 2 and 3: k == 1 and k == n are both legal windows.
        CHECK((alarms({1, 2}, 1, 1) == std::vector<int>{1}));
        CHECK((alarms({1, 2, 3}, 3, 5) == std::vector<int>{2}));

        // Clause 5: right-end indexes, ascending, once per window.
        CHECK((alarms({1, 2, 3, 4}, 2, 4) == std::vector<int>{2, 3}));
        CHECK((alarms({1, 2, 3, 4}, 2, 0) == std::vector<int>{1, 2, 3}));

        // Clause 6: wide sums. 200000 frames of 10^9 reach 2*10^14, far beyond
        // a 32-bit accumulator but within the required range.
        {
            std::vector<int> big(200000, 1000000000);
            std::vector<int> hits = alarms(big, 200000, 199999999999999LL);
            CHECK((hits == std::vector<int>{199999}));
            CHECK(alarms(big, 200000, 200000000000000LL).empty());
        }

        // All-negative input stays correct when every window is below zero.
        CHECK(alarms({-1, -2, -3}, 2, 0).empty());

        std::cout << "Q3 local boundary tests: PASS\n";
    } catch (const std::exception& e) {
        std::cerr << "Q3 local boundary tests: FAIL: " << e.what() << "\n";
        return 1;
    }
    return 0;
}
