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

// Assessment 01 B2; see 题面.pdf.
namespace aim::b2 {


// BEGIN IMPLEMENTATION
std::size_t compact(int* data, std::size_t n, int low, int high) {
    // Reversed bounds retain nothing and must leave every element untouched.
    if (low > high) return 0;
    // Single write pointer: data[0..kept-1] holds the retained values in sampling order,
    // and kept<=i always, so data[i] is read before it can be overwritten.
    std::size_t kept = 0;
    for (std::size_t i = 0; i < n; ++i) {
        if (data[i] >= low && data[i] <= high) {
            data[kept] = data[i];
            ++kept;
        }
    }
    return kept;
}
// END IMPLEMENTATION
}

#define CHECK(...) do { if (!(__VA_ARGS__)) throw std::runtime_error("check failed"); } while(false)
#define EXPECT_INVALID(...) do { bool caught=false; try { (void)(__VA_ARGS__); } catch(const std::invalid_argument&) { caught=true; } CHECK(caught); } while(false)
#define EXPECT_RANGE(...) do { bool caught=false; try { (void)(__VA_ARGS__); } catch(const std::out_of_range&) { caught=true; } CHECK(caught); } while(false)

using namespace aim::b2;
int main() {
    try {
        { int a[] = {-1,2,0,9}; std::size_t n=compact(a,4,0,2); CHECK(n==2 && a[0]==2 && a[1]==0); }
        { CHECK(compact(nullptr,0,0,1)==0); }
        std::cout << "PUBLIC 2/2\n";
    } catch(const std::exception& e) {
        std::cerr << e.what() << "\n";
        return 1;
    }
    return 0;
}
