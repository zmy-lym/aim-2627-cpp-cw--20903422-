// Local boundary tests for Q5. Not part of the submitted implementation:
// the graded region lives in src/main/A1_q5.cpp only.
#define main aim_local_q5_embedded_main
#include "../src/main/A1_q5.cpp"
#undef main

using Matrix = std::vector<std::vector<int>>;

// True only when std::invalid_argument is thrown; any other outcome fails.
static bool rejects(const std::vector<std::string>& grid) {
    try {
        (void)risk(grid);
    } catch (const std::invalid_argument&) {
        return true;
    } catch (...) {
        return false;
    }
    return false;
}

int main() {
    try {
        // Clause 1: an empty row sequence is valid, and returns an empty matrix.
        CHECK(risk({}) == Matrix{});
        CHECK(rejects({""}));
        CHECK(rejects({"..", "."}));

        // Clause 2: only '.' and '#' are legal; the check is a whitelist, so
        // spaces, letters and punctuation are all rejected.
        CHECK(rejects({"A"}));
        CHECK(rejects({" "}));
        CHECK(rejects({"./."}));
        CHECK(rejects({".#", ".*"}));

        // Clause 3: the whole map is validated first, so a bad character in a
        // later row still rejects the map.
        CHECK(rejects({"..", "..", ".A"}));
        CHECK(rejects({"..", "..", "..", "..", "..", "..", "..", "..", "..", "..",
                       "..", "..", "..", "..", "..", "..", "..", "..", "..", "..",
                       ".Z"}));

        // Clause 4: obstacles are -1, free cells count orthogonal obstacles.
        CHECK((risk({".#", ".."}) == Matrix{{1, -1}, {0, 1}}));
        CHECK((risk({"."}) == Matrix{{0}}));
        CHECK((risk({"#", "#"}) == Matrix{{-1}, {-1}}));
        CHECK((risk({"#.#"}) == Matrix{{-1, 2, -1}}));

        // Clause 5: diagonals are excluded. The centre of this ring is free and
        // touches only diagonal obstacles, so its risk must be exactly 0.
        {
            Matrix m = risk({"#.#", "...", "#.#"});
            CHECK(m[1][1] == 0);
            CHECK((m == Matrix{{-1, 2, -1}, {2, 0, 2}, {-1, 2, -1}}));
        }

        // Clause 5: edges add no risk, and the maximum interior risk is 4.
        CHECK((risk({"..#"}) == Matrix{{0, 1, -1}}));
        CHECK((risk({"..", ".."}) == Matrix{{0, 0}, {0, 0}}));
        {
            Matrix m = risk({"###", "#.#", "###"});
            CHECK(m[1][1] == 4);
        }

        // Clause 6: the input is left unchanged and the output keeps the same
        // dimensions as the input.
        {
            std::vector<std::string> grid = {".#.", "#.#", ".#."};
            std::vector<std::string> before = grid;
            Matrix m = risk(grid);
            CHECK(grid == before);
            CHECK(m.size() == 3 && m[0].size() == 3);
        }

        // Clause 1: the 300x300 upper bound stays correct in both extremes.
        {
            Matrix all_free = risk(std::vector<std::string>(300, std::string(300, '.')));
            CHECK(all_free.size() == 300 && all_free[0].size() == 300);
            CHECK(all_free[0][0] == 0 && all_free[299][299] == 0);
            Matrix all_blocked = risk(std::vector<std::string>(300, std::string(300, '#')));
            CHECK(all_blocked[0][0] == -1 && all_blocked[299][299] == -1);
        }

        std::cout << "Q5 local boundary tests: PASS\n";
    } catch (const std::exception& e) {
        std::cerr << "Q5 local boundary tests: FAIL: " << e.what() << "\n";
        return 1;
    }
    return 0;
}
