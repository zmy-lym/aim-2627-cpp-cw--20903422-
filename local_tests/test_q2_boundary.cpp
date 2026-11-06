// Local boundary tests for Q2. Not part of the submitted implementation:
// the graded region lives in src/main/A1_q2.cpp only.
#define main aim_local_q2_embedded_main
#include "../src/main/A1_q2.cpp"
#undef main

int main() {
    try {
        // Clause 4: only a complete field matches, so cam never touches cam2,
        // and position zero is a legitimate match.
        CHECK(edit_labels("cam2,cam", {{"cam", "imu"}}) == "cam2,imu");
        CHECK(edit_labels("cam,cam2,cam", {{"cam", "camera"}, {"cam", ""}})
              == "camera,cam2");

        // Clause 4: only the first occurrence is edited, even for from == to.
        CHECK(edit_labels("a,b,a", {{"a", "x"}}) == "x,b,a");
        CHECK(edit_labels("a,b,a", {{"a", "a"}}) == "a,b,a");

        // Clause 5: deletion removes the necessary adjacent comma at every
        // position, including the only label.
        CHECK(edit_labels("a,b", {{"b", ""}}) == "a");
        CHECK(edit_labels("a,b", {{"a", ""}}) == "b");
        CHECK(edit_labels("a,b,c", {{"b", ""}}) == "a,c");
        CHECK(edit_labels("a", {{"a", ""}}) == "");
        CHECK(edit_labels("a", {{"a", ""}, {"", "b"}}) == "b");

        // Clause 3: appending never introduces a leading comma.
        CHECK(edit_labels("", {{"", "cam"}}) == "cam");
        CHECK(edit_labels("a", {{"", "b"}}) == "a,b");
        CHECK(edit_labels("", {{"", ""}}) == "");

        // Clause 4: a missing label leaves the text unchanged.
        CHECK(edit_labels("cam,imu", {{"gz", "x"}}) == "cam,imu");

        // Clause 6: duplicates are allowed and never deduplicated.
        CHECK(edit_labels("a,a,a", {{"", "a"}}) == "a,a,a,a");

        // Clause 6: later edits must search the updated text, not cached
        // positions, so a length-changing rename cannot shift a later match.
        CHECK(edit_labels("cam,imu", {{"cam", "camera"}, {"imu", "gyro"}})
              == "camera,gyro");
        CHECK(edit_labels("a,bb", {{"a", "alonger"}, {"bb", ""}}) == "alonger");

        // Clause 7: empty edits returns the original text.
        CHECK(edit_labels("cam,imu", {}) == "cam,imu");

        std::cout << "Q2 local boundary tests: PASS\n";
    } catch (const std::exception& e) {
        std::cerr << "Q2 local boundary tests: FAIL: " << e.what() << "\n";
        return 1;
    }
    return 0;
}
