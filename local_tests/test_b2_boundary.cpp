// Local boundary tests for B2. Not part of the submitted implementation:
// the graded region lives in src/main/A1_b2.cpp only.
#define main aim_local_b2_embedded_main
#include "../src/main/A1_b2.cpp"
#undef main

int main() {
    try {
        // Clause 2 and 3: retained values keep their sampling order, so this is
        // 2 then 0, not the sorted 0 then 2.
        {
            int data[] = {-1, 2, 0, 9};
            std::size_t k = compact(data, 4, 0, 2);
            CHECK(k == 2 && data[0] == 2 && data[1] == 0);
        }

        // Clause 1: n == 0 may pass nullptr and must never dereference it.
        CHECK(compact(nullptr, 0, 0, 1) == 0);

        // Clause 2 and 6: duplicates are kept with O(1) extra space.
        {
            int data[] = {3, 1, 3, 2, 3};
            std::size_t k = compact(data, 5, 2, 3);
            CHECK(k == 4 && data[0] == 3 && data[1] == 3 && data[2] == 2 && data[3] == 3);
        }

        // Clause 5: reversed bounds retain nothing and modify no element.
        {
            int data[] = {1, 2};
            std::size_t k = compact(data, 2, 5, 3);
            CHECK(k == 0 && data[0] == 1 && data[1] == 2);
        }

        // Clause 2: equal bounds are valid and inclusive.
        {
            int data[] = {1, 1, 1};
            CHECK(compact(data, 3, 1, 1) == 3);
        }

        // Clause 2 and 5: retaining everything and retaining nothing.
        {
            int data[] = {5, 6, 7};
            std::size_t k = compact(data, 3, 0, 100);
            CHECK(k == 3 && data[0] == 5 && data[1] == 6 && data[2] == 7);
        }
        {
            int data[] = {1, 2, 3};
            CHECK(compact(data, 3, 10, 20) == 0);
        }

        // Clause 3: stability survives a long run of mixed values. The expected
        // prefix is built from a copy, because compact rewrites data in place.
        {
            std::vector<int> data;
            for (int i = 0; i < 1000; ++i) data.push_back(i % 7 - 3);
            std::vector<int> original = data;
            std::vector<int> expected;
            for (int v : original) {
                if (v >= -1 && v <= 1) expected.push_back(v);
            }
            std::size_t k = compact(data.data(), data.size(), -1, 1);
            CHECK(k == expected.size());
            bool stable = true;
            for (std::size_t i = 0; i < k; ++i) {
                if (data[i] != expected[i]) stable = false;
            }
            CHECK(stable);
        }

        // Clause 4: the tail past the returned length is unspecified, so it is
        // never compared. Only the prefix belongs to the result.
        {
            int data[] = {9, 1, 9, 2, 9};
            std::size_t k = compact(data, 5, 1, 2);
            CHECK(k == 2 && data[0] == 1 && data[1] == 2);
        }

        std::cout << "B2 local boundary tests: PASS\n";
    } catch (const std::exception& e) {
        std::cerr << "B2 local boundary tests: FAIL: " << e.what() << "\n";
        return 1;
    }
    return 0;
}
