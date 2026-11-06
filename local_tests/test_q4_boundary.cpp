// Local boundary tests for Q4. Not part of the submitted implementation:
// the graded region lives in src/main/A1_q4.cpp only.
#define main aim_local_q4_embedded_main
#include "../src/main/A1_q4.cpp"
#undef main

// Convenience wrapper so every case reads as one line.
static bool same(const CommandResult& r, long long v, int a, int rj) {
    return r.value == v && r.accepted == a && r.rejected == rj;
}

int main() {
    try {
        // Clause 2: operations are case-sensitive and an empty line is rejected.
        CHECK(same(execute({"add 5", ""}), 0, 0, 2));

        // Clause 3: the argument range is [-10^6,10^6], inclusive. This is the
        // defect that was fixed: values above 10^6 used to be accepted.
        CHECK(same(execute({"SET 1000000"}), 1000000, 1, 0));
        CHECK(same(execute({"SET -1000000"}), -1000000, 1, 0));
        CHECK(same(execute({"SET 1000001"}), 0, 0, 1));
        CHECK(same(execute({"SET -1000001"}), 0, 0, 1));
        CHECK(same(execute({"ADD 5000000"}), 0, 0, 1));

        // Clause 4: a valid argument can still propose an out-of-range result.
        CHECK(same(execute({"SET 1000000", "ADD 1"}), 1000000, 1, 1));

        // Clause 5: rejected commands preserve the accumulator, and accepted
        // ones really do increment the counter.
        CHECK(same(execute({"SET 5", "RESET x"}), 5, 1, 1));
        CHECK(same(execute({"ADD 1", "ADD 2", "ADD 3"}), 6, 3, 0));
        CHECK(same(execute({"RESET", "RESET", "RESET"}), 0, 3, 0));

        // Clause 3: malformed arguments and unknown operations are rejected.
        CHECK(same(execute({"ADD"}), 0, 0, 1));
        CHECK(same(execute({"ADD 5 6"}), 0, 0, 1));
        CHECK(same(execute({"ADD 12x"}), 0, 0, 1));
        CHECK(same(execute({"ADD 1.5"}), 0, 0, 1));
        CHECK(same(execute({"RESET now"}), 0, 0, 1));
        CHECK(same(execute({"MUL 5"}), 0, 0, 1));
        CHECK(same(execute({"   "}), 0, 0, 1));
        CHECK(same(execute({"ADD 99999999999999999999"}), 0, 0, 1));

        // Clause 2: surrounding stream whitespace is allowed.
        CHECK(same(execute({"  ADD   5  "}), 5, 1, 0));

        // Clause 6: the counting invariant and independence between calls.
        {
            std::vector<std::string> lines = {"ADD 5", "bogus", "SET 7", "ADD 1 x",
                                              "RESET", "", "ADD -3"};
            CommandResult r = execute(lines);
            CHECK(r.accepted + r.rejected == static_cast<int>(lines.size()));
            CHECK(same(r, -3, 4, 3));
        }
        CHECK(same(execute({}), 0, 0, 0));

        std::cout << "Q4 local boundary tests: PASS\n";
    } catch (const std::exception& e) {
        std::cerr << "Q4 local boundary tests: FAIL: " << e.what() << "\n";
        return 1;
    }
    return 0;
}
