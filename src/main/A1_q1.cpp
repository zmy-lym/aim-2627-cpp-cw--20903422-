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
    long long bounded = std::clamp(hp, 0LL, max_hp);
    return bounded * 100 / max_hp;
 }
std::string report(int id, long long hp, long long max_hp, int battery) { 
    int charge = std::clamp(battery, 0, 100);
    std::string state = charge >= 60 ? "OK" : (charge >= 20 ? "WARNING" : "LOW");
    long long percent = ratio(hp, max_hp);
    std::string hp_text = percent == -1 ? "INVALID" : std::to_string(percent);
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
