// Local boundary tests for B1. Not part of the submitted implementation:
// the graded region lives in src/main/A1_b1.cpp only.
#define main aim_local_b1_embedded_main
#include "../src/main/A1_b1.cpp"
#undef main

int main() {
    try {
        // Clause 3: both limits are inclusive. The negative limit is asymmetric,
        // which is the defect this suite was written to catch: a single shared
        // magnitude limit used to reject -2147483648.
        {
            int v = 0;
            CHECK(parse_i32("2147483647", v) && v == 2147483647);
        }
        {
            int v = 0;
            CHECK(parse_i32("-2147483648", v) && v == -2147483647 - 1);
        }

        // Clause 3: one step beyond either limit fails.
        {
            int v = 9;
            CHECK(!parse_i32("2147483648", v) && v == 9);
        }
        {
            int v = 9;
            CHECK(!parse_i32("-2147483649", v) && v == 9);
        }

        // Clause 4: every failure leaves the caller's variable untouched.
        {
            int v = 9;
            CHECK(!parse_i32("", v) && v == 9);
            CHECK(!parse_i32("+", v) && v == 9);
            CHECK(!parse_i32("-", v) && v == 9);
            CHECK(!parse_i32(" 1", v) && v == 9);
            CHECK(!parse_i32("1 ", v) && v == 9);
            CHECK(!parse_i32("1.0", v) && v == 9);
            CHECK(!parse_i32("1 2", v) && v == 9);
            CHECK(!parse_i32("--1", v) && v == 9);
            CHECK(!parse_i32("+-1", v) && v == 9);
            CHECK(!parse_i32("12x", v) && v == 9);
            CHECK(!parse_i32("x12", v) && v == 9);
            CHECK(!parse_i32(std::string(1000, '9'), v) && v == 9);
        }

        // Clause 2: leading zeros, +0 and -0 are all accepted.
        {
            int v = 9;
            CHECK(parse_i32("0000000000000000000007", v) && v == 7);
            CHECK(parse_i32("+0", v) && v == 0);
            CHECK(parse_i32("-0", v) && v == 0);
            CHECK(parse_i32("-0000", v) && v == 0);
        }
        {
            int v = 9;
            CHECK(parse_i32("+42", v) && v == 42);
            CHECK(parse_i32("-42", v) && v == -42);
        }

        std::cout << "B1 local boundary tests: PASS\n";
    } catch (const std::exception& e) {
        std::cerr << "B1 local boundary tests: FAIL: " << e.what() << "\n";
        return 1;
    }
    return 0;
}
