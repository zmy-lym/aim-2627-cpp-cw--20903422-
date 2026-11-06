// Local boundary tests for Q7. Not part of the submitted implementation:
// the graded region lives in src/main/A1_q7.cpp only.
#define main aim_local_q7_embedded_main
#include "../src/main/A1_q7.cpp"
#undef main

// Field-by-field comparison keeps a failure message easy to read.
static bool same(const FireResult& r, int shots, int rejected, int ammo, int heat) {
    return r.shots == shots && r.rejected == rejected
        && r.ammo == ammo && r.heat == heat;
}

int main() {
    try {
        // Clause 6: no frames means no cooling, so the initial resources survive.
        CHECK(same(simulate(4, 80, 10, 30, {}), 0, 0, 4, 80));
        CHECK(same(simulate(0, 0, 0, 0, {}), 0, 0, 0, 0));

        // Clause 2: cooling happens first, so the request is judged against the
        // cooled heat. An empty magazine is still rejected after cooling.
        CHECK(same(simulate(0, 50, 10, 20, {1}), 0, 1, 0, 40));
        CHECK(same(simulate(3, 100, 20, 10, {0}), 0, 0, 3, 80));

        // Clause 4: heat exactly 100 is allowed, and zero shot heat is valid.
        CHECK(same(simulate(1, 100, 0, 0, {1}), 1, 0, 0, 100));
        CHECK(same(simulate(1, 99, 0, 1, {1}), 1, 0, 0, 100));

        // Clause 5: a rejection consumes neither ammunition nor shot heat.
        CHECK(same(simulate(1, 100, 0, 1, {1}), 0, 1, 1, 100));
        CHECK(same(simulate(1, 99, 0, 2, {1}), 0, 1, 1, 99));

        // Clause 2: heat never drops below zero.
        CHECK(same(simulate(5, 10, 100, 20, {1}), 1, 0, 4, 20));
        CHECK(same(simulate(5, 0, 0, 0, {0, 0, 0}), 0, 0, 5, 0));

        // Clause 3: a zero request only cools.
        CHECK(same(simulate(2, 90, 10, 20, {1, 1, 1}), 2, 1, 0, 100));

        // Clause 6: the two counters account for every request.
        {
            std::vector<int> ones(10000, 1);
            FireResult r = simulate(10000, 0, 0, 1, ones);
            CHECK(r.shots + r.rejected == 10000);
            CHECK(r.ammo + r.shots == 10000);
        }
        {
            std::vector<int> zeros(10000, 0);
            CHECK(same(simulate(7, 100, 1, 5, zeros), 0, 0, 7, 0));
        }

        std::cout << "Q7 local boundary tests: PASS\n";
    } catch (const std::exception& e) {
        std::cerr << "Q7 local boundary tests: FAIL: " << e.what() << "\n";
        return 1;
    }
    return 0;
}
