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

// Assessment 01 Q1; see 题面.pdf.
namespace aim::q1 {


// BEGIN IMPLEMENTATION
long long ratio(long long hp, long long max_hp) {
    if (max_hp <= 0) return -1;
    // Clamp health into [0,max_hp] before scaling; max_hp>0 so no division by zero.
    long long bounded = hp;
    if (bounded < 0) bounded = 0;
    if (bounded > max_hp) bounded = max_hp;
    // bounded fits in [0,10^12], so the intermediate product stays far below LLONG_MAX.
    return bounded * 100 / max_hp;
}
std::string report(int id, long long hp, long long max_hp, int battery) {
    // Normalize battery to [0,100] before classifying, so negative values become LOW.
    int charge = battery;
    if (charge < 0) charge = 0;
    if (charge > 100) charge = 100;
    std::string state;
    if (charge >= 60) {
        state = "OK";
    } else if (charge >= 20) {
        state = "WARNING";
    } else {
        state = "LOW";
    }
    // ratio returns -1 only when max_hp<=0; otherwise the percentage is nonnegative.
    const long long percent = ratio(hp, max_hp);
    std::string hp_text;
    if (percent < 0) {
        hp_text = "INVALID";
    } else {
        hp_text = std::to_string(percent);
    }
    return "ID=" + std::to_string(id) + ";HP=" + hp_text + ";BAT=" + std::to_string(charge) + ";STATE=" + state;
}
// END IMPLEMENTATION
}

#define CHECK(...) do { if (!(__VA_ARGS__)) throw std::runtime_error("check failed"); } while(false)
#define EXPECT_INVALID(...) do { bool caught=false; try { (void)(__VA_ARGS__); } catch(const std::invalid_argument&) { caught=true; } CHECK(caught); } while(false)
#define EXPECT_RANGE(...) do { bool caught=false; try { (void)(__VA_ARGS__); } catch(const std::out_of_range&) { caught=true; } CHECK(caught); } while(false)

using namespace aim::q1;
int main() {
    try {
        { CHECK(ratio(2,3)==66); }
        { CHECK(report(7,2,3,20)=="ID=7;HP=66;BAT=20;STATE=WARNING"); }
        std::cout << "PUBLIC 2/2\n";
    } catch(const std::exception& e) {
        std::cerr << e.what() << "\n";
        return 1;
    }
    return 0;
}
