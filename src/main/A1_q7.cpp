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

// Assessment 01 Q7; see 题面.pdf.
namespace aim::q7 {
struct FireResult { int shots; int rejected; int ammo; int heat; };

// BEGIN IMPLEMENTATION
FireResult simulate(int ammo, int heat, int cool, int shot_heat, const std::vector<int>& requests) {
    // Counters start at zero; ammo and heat carry the caller's initial resources.
    int shots = 0;
    int rejected = 0;
    for (std::size_t i = 0; i < requests.size(); ++i) {
        // Every frame cools first, so a request is judged against the cooled heat.
        heat -= cool;
        if (heat < 0) heat = 0;
        // A zero request only cools; it never consumes ammunition or counts anywhere.
        if (requests[i] != 1) continue;
        // A shot needs a round and must not push heat above 100; equality is allowed.
        if (ammo > 0 && heat + shot_heat <= 100) {
            --ammo;
            heat += shot_heat;
            ++shots;
        } else {
            // Rejection consumes nothing: neither ammunition nor shot heat.
            ++rejected;
        }
    }
    FireResult result;
    result.shots = shots;
    result.rejected = rejected;
    result.ammo = ammo;
    result.heat = heat;
    return result;
}
// END IMPLEMENTATION
}

#define CHECK(...) do { if (!(__VA_ARGS__)) throw std::runtime_error("check failed"); } while(false)
#define EXPECT_INVALID(...) do { bool caught=false; try { (void)(__VA_ARGS__); } catch(const std::invalid_argument&) { caught=true; } CHECK(caught); } while(false)
#define EXPECT_RANGE(...) do { bool caught=false; try { (void)(__VA_ARGS__); } catch(const std::out_of_range&) { caught=true; } CHECK(caught); } while(false)

using namespace aim::q7;
int main() {
    try {
        { aim::q7::FireResult r=simulate(2,90,10,20,{1,1,1}); CHECK(r.shots==2 && r.rejected==1 && r.ammo==0 && r.heat==100); }
        { aim::q7::FireResult r=simulate(3,100,20,10,{0}); CHECK(r.heat==80 && r.rejected==0 && r.ammo==3); }
        std::cout << "PUBLIC 2/2\n";
    } catch(const std::exception& e) {
        std::cerr << e.what() << "\n";
        return 1;
    }
    return 0;
}
