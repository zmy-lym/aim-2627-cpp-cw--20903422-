// Local boundary tests for Q8. Not part of the submitted implementation:
// the graded region lives in src/main/A1_q8.cpp only.
#define main aim_local_q8_embedded_main
#include "../src/main/A1_q8.cpp"
#undef main

#include <set>

// Clause 6: the two count identities must always hold.
static bool invariants(const Summary& s, std::size_t n) {
    return s.valid + s.duplicate + s.invalid == static_cast<int>(n)
        && static_cast<int>(s.ids.size()) == s.valid;
}

int main() {
    try {
        // Clause 6: an empty batch starts fresh.
        {
            Summary s = summarize({});
            CHECK(s.valid == 0 && s.duplicate == 0 && s.invalid == 0);
            CHECK(s.total_hp == 0 && s.ids.empty());
            CHECK(invariants(s, 0));
        }

        // Clause 2: an invalid record reserves no id, so the next record with
        // the same id is still a first valid one.
        {
            Summary s = summarize({{1, -1001}, {1, 30}});
            CHECK(s.valid == 1 && s.invalid == 1 && s.total_hp == 30);
            CHECK((s.ids == std::vector<int>{1}));
            CHECK(invariants(s, 2));
        }

        // Clause 3: validation comes before deduplication, so an out-of-range
        // record for an already seen id counts as invalid, not duplicate.
        {
            Summary s = summarize({{1, 20}, {1, -1001}});
            CHECK(s.valid == 1 && s.duplicate == 0 && s.invalid == 1);
            CHECK(invariants(s, 2));
        }

        // Clause 5: ids come back ascending, not in arrival order.
        {
            Summary s = summarize({{2, 5}, {1, 9}});
            CHECK((s.ids == std::vector<int>{1, 2}));
            CHECK(s.total_hp == 14);
            CHECK(invariants(s, 2));
        }
        {
            Summary s = summarize({{9000, 1}, {7, 1}, {500, 1}, {1, 1}});
            CHECK((s.ids == std::vector<int>{1, 7, 500, 9000}));
            CHECK(invariants(s, 4));
        }

        // Clause 1 and 2: id and hp range endpoints are inclusive, and both
        // fields are checked.
        {
            Summary s = summarize({{0, 0}, {9999, 1000}, {0, 5}});
            CHECK(s.valid == 2 && s.duplicate == 1 && s.invalid == 0);
            CHECK(s.total_hp == 100);
            CHECK((s.ids == std::vector<int>{0, 9999}));
            CHECK(invariants(s, 3));
        }
        {
            Summary s = summarize({{10000, 5}, {-1, 5}, {5, -1001}, {5, 1001}});
            CHECK(s.valid == 0 && s.duplicate == 0 && s.invalid == 4);
            CHECK(invariants(s, 4));
        }

        // Clause 4: the input range is validated first, then health is clamped
        // to [0,100] only for the running total.
        {
            Summary s = summarize({{4, 1000}});
            CHECK(s.valid == 1 && s.total_hp == 100);
        }
        {
            Summary s = summarize({{4, -1}});
            CHECK(s.valid == 1 && s.total_hp == 0);
        }
        {
            Summary s = summarize({{2, 200}, {1, -5}, {2, 10}, {-1, 30}});
            CHECK(s.valid == 2 && s.duplicate == 1 && s.invalid == 1);
            CHECK(s.total_hp == 100);
            CHECK((s.ids == std::vector<int>{1, 2}));
            CHECK(invariants(s, 4));
        }

        // Clause 6: later valid packets never replace the first accepted value,
        // and no state leaks between consecutive calls.
        {
            Summary s = summarize({{9, 10}, {9, 20}, {9, 30}});
            CHECK(s.valid == 1 && s.duplicate == 2 && s.total_hp == 10);
        }
        {
            Summary a = summarize({{7, 50}});
            Summary b = summarize({{7, 50}});
            CHECK(a.valid == 1 && b.valid == 1 && b.duplicate == 0);
            CHECK(b.total_hp == 50);
        }

        // A mixed batch is checked against a naive reference that reads the
        // clauses directly, so the two identities are verified independently.
        {
            std::vector<Telemetry> batch = {{3, 40}, {3, 90}, {-2, 10}, {2, 200},
                                            {10000, 1}, {2, 5}, {3, 1001}, {0, -7}};
            Summary got = summarize(batch);
            Summary want{0, 0, 0, 0, {}};
            std::set<int> seen;
            for (const Telemetry& t : batch) {
                if (t.id < 0 || t.id > 9999 || t.hp < -1000 || t.hp > 1000) {
                    ++want.invalid;
                    continue;
                }
                if (seen.count(t.id)) {
                    ++want.duplicate;
                    continue;
                }
                seen.insert(t.id);
                ++want.valid;
                want.total_hp += t.hp < 0 ? 0 : (t.hp > 100 ? 100 : t.hp);
            }
            want.ids.assign(seen.begin(), seen.end());
            CHECK(got.valid == want.valid && got.duplicate == want.duplicate);
            CHECK(got.invalid == want.invalid && got.total_hp == want.total_hp);
            CHECK(got.ids == want.ids);
            CHECK(invariants(got, batch.size()));
        }

        std::cout << "Q8 local boundary tests: PASS\n";
    } catch (const std::exception& e) {
        std::cerr << "Q8 local boundary tests: FAIL: " << e.what() << "\n";
        return 1;
    }
    return 0;
}
