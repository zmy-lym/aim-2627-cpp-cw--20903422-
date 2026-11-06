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

// Assessment 01 Q6; see 题面.pdf.
namespace aim::q6 {
struct Target { int id; int x; int y; int hp; bool visible; };

// BEGIN IMPLEMENTATION
int select_target(const std::vector<Target>& targets, int max_range) {
    // A negative range admits no candidate, not even one sitting at the origin.
    if (max_range < 0) return -1;
    // Squared distances reach 2*10^12 and the squared range reaches 10^12,
    // so every intermediate product needs a 64-bit type rather than int.
    const long long range = static_cast<long long>(max_range);
    const long long range_sq = range * range;
    bool found = false;
    int best_id = 0;
    int best_hp = 0;
    long long best_dist_sq = 0;
    for (std::size_t i = 0; i < targets.size(); ++i) {
        const Target& target = targets[i];
        // Invisible or dead targets never qualify, whatever their position.
        if (!target.visible || target.hp <= 0) continue;
        const long long dx = static_cast<long long>(target.x);
        const long long dy = static_cast<long long>(target.y);
        const long long dist_sq = dx * dx + dy * dy;
        // Range uses Euclidean distance squared, and the boundary is inclusive.
        if (dist_sq > range_sq) continue;
        bool better = false;
        if (!found) {
            better = true;
        } else if (target.hp != best_hp) {
            // Lower health outranks everything else.
            better = target.hp < best_hp;
        } else if (dist_sq != best_dist_sq) {
            // Equal health falls back to the nearer target.
            better = dist_sq < best_dist_sq;
        } else {
            // Full tie: the smaller id wins, so input order never decides.
            better = target.id < best_id;
        }
        if (better) {
            found = true;
            best_id = target.id;
            best_hp = target.hp;
            best_dist_sq = dist_sq;
        }
    }
    return found ? best_id : -1;
}
// END IMPLEMENTATION
}

#define CHECK(...) do { if (!(__VA_ARGS__)) throw std::runtime_error("check failed"); } while(false)
#define EXPECT_INVALID(...) do { bool caught=false; try { (void)(__VA_ARGS__); } catch(const std::invalid_argument&) { caught=true; } CHECK(caught); } while(false)
#define EXPECT_RANGE(...) do { bool caught=false; try { (void)(__VA_ARGS__); } catch(const std::out_of_range&) { caught=true; } CHECK(caught); } while(false)

using namespace aim::q6;
int main() {
    try {
        { CHECK(select_target({{7,3,4,20,true},{2,0,5,20,true}},5)==2); }
        { CHECK(select_target({},5)==-1); }
        std::cout << "PUBLIC 2/2\n";
    } catch(const std::exception& e) {
        std::cerr << e.what() << "\n";
        return 1;
    }
    return 0;
}
