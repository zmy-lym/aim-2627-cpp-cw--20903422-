// Local boundary tests for Q1. Not part of the submitted implementation:
// the graded region lives in src/main/A1_q1.cpp only.
//
// The implementation file is included with its `main` renamed, the same trick
// src/tests uses, so the published CHECK macro and the aim::q1 namespace are
// reused instead of duplicated.
#define main aim_local_q1_embedded_main
#include "../src/main/A1_q1.cpp"
#undef main

int main() {
    try {
        // Clause 2: an invalid maximum returns -1 without dividing, while the
        // rest of the report still proceeds.
        CHECK(ratio(10, 0) == -1);
        CHECK(ratio(10, -5) == -1);
        CHECK(report(0, 4, 0, -7) == "ID=0;HP=INVALID;BAT=0;STATE=LOW");

        // Clause 3: clamp before scaling, truncate rather than round.
        CHECK(ratio(-5, 100) == 0);
        CHECK(ratio(200, 100) == 100);
        CHECK(ratio(2, 3) == 66);
        CHECK(ratio(1, 3) == 33);

        // Clause 4: classify only after the battery is normalised.
        CHECK(report(1, 0, 100, 19).find("STATE=LOW") != std::string::npos);
        CHECK(report(1, 0, 100, 20).find("STATE=WARNING") != std::string::npos);
        CHECK(report(1, 0, 100, 59).find("STATE=WARNING") != std::string::npos);
        CHECK(report(1, 0, 100, 60).find("STATE=OK") != std::string::npos);
        CHECK(report(1, 0, 100, -7).find("BAT=0;") != std::string::npos);
        CHECK(report(1, 0, 100, 150).find("BAT=100;") != std::string::npos);

        // Clause 5: no zero padding, no spaces, no trailing newline.
        CHECK(report(7, 2, 3, 20) == "ID=7;HP=66;BAT=20;STATE=WARNING");

        // Clause 1: magnitudes up to 10^12 must not overflow the intermediate
        // product. full health and a tiny-but-positive maximum are both checked.
        CHECK(ratio(1000000000000LL, 1000000000000LL) == 100);
        CHECK(ratio(999999999999LL, 1) == 100);
        CHECK(report(9999, 1000000000000LL, 1000000000000LL, 100)
              == "ID=9999;HP=100;BAT=100;STATE=OK");

        std::cout << "Q1 local boundary tests: PASS\n";
    } catch (const std::exception& e) {
        std::cerr << "Q1 local boundary tests: FAIL: " << e.what() << "\n";
        return 1;
    }
    return 0;
}
