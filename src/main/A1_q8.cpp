#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <iostream>
#include <limits>
#include <memory>
#include <numeric>
#include <sstream>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

// Assessment 01 Q8; see 题面.pdf.
namespace aim::q8 {
struct Telemetry { int id; int hp; };
struct Summary { int valid; int duplicate; int invalid; long long total_hp; std::vector<int> ids; };

// BEGIN IMPLEMENTATION
Summary summarize(const std::vector<Telemetry>& frames) {
    Summary summary;
    summary.valid = 0;
    summary.duplicate = 0;
    summary.invalid = 0;
    summary.total_hp = 0;
    // Ids are bounded to [0,9999], so a 10000-entry marker table can record the
    // first accepted record for every legal id without sorting or extra state.
    const int kMaxId = 9999;
    std::vector<bool> seen(static_cast<std::size_t>(kMaxId) + 1, false);
    for (std::size_t i = 0; i < frames.size(); ++i) {
        const Telemetry& frame = frames[i];
        // Validate both fields before touching the marker table, so an out-of-range
        // id can never index it and an invalid record never reserves an id.
        const bool id_ok = frame.id >= 0 && frame.id <= kMaxId;
        const bool hp_ok = frame.hp >= -1000 && frame.hp <= 1000;
        if (!id_ok || !hp_ok) {
            ++summary.invalid;
            continue;
        }
        // Duplicates are only recognised after full validation, so a legal record
        // repeating a seen id counts as duplicate while an illegal one stays invalid.
        if (seen[static_cast<std::size_t>(frame.id)]) {
            ++summary.duplicate;
            continue;
        }
        seen[static_cast<std::size_t>(frame.id)] = true;
        ++summary.valid;
        // Input range [-1000,1000] is validated above; the total uses health
        // clamped to [0,100] instead, so legal negative health contributes nothing.
        int normalized = frame.hp;
        if (normalized < 0) normalized = 0;
        if (normalized > 100) normalized = 100;
        summary.total_hp += normalized;
    }
    // Ids are emitted by scanning the marker table in ascending order, never in
    // arrival order, so the result is sorted regardless of the input sequence.
    for (int id = 0; id <= kMaxId; ++id) {
        if (seen[static_cast<std::size_t>(id)]) summary.ids.push_back(id);
    }
    return summary;
}
// END IMPLEMENTATION
}

#define CHECK(...) do { if (!(__VA_ARGS__)) throw std::runtime_error("check failed"); } while(false)
#define EXPECT_INVALID(...) do { bool caught=false; try { (void)(__VA_ARGS__); } catch(const std::invalid_argument&) { caught=true; } CHECK(caught); } while(false)
#define EXPECT_RANGE(...) do { bool caught=false; try { (void)(__VA_ARGS__); } catch(const std::out_of_range&) { caught=true; } CHECK(caught); } while(false)

using namespace aim::q8;
int main() {
    try {
        { aim::q8::Summary r=summarize({{2,200},{1,-5},{2,10},{-1,30}}); CHECK(r.valid==2 && r.duplicate==1 && r.invalid==1 && r.total_hp==100 && (r.ids==std::vector<int>{1,2})); }
        { aim::q8::Summary r=summarize({{1,-1001},{1,30},{1,1001}}); CHECK(r.valid==1 && r.duplicate==0 && r.invalid==2 && r.total_hp==30 && (r.ids==std::vector<int>{1})); }
        std::cout << "PUBLIC 2/2\n";
    } catch(const std::exception& e) {
        std::cerr << e.what() << "\n";
        return 1;
    }
    return 0;
}
