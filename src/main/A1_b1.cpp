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

// Assessment 01 B1; see 题面.pdf.
namespace aim::b1 {


// BEGIN IMPLEMENTATION
bool parse_i32(const std::string& text, int& output) {
    if (text.empty()) return false;
    std::size_t pos = 0;
    bool negative = false;
    if (text[pos] == '-' || text[pos] == '+') {
        negative = text[pos] == '-';
        ++pos;
    }
    // A sign with no digits is invalid.
    if (pos == text.size()) return false;
    // The int limits are asymmetric: 2147483648 is legal only with a leading '-',
    // so accumulate the magnitude and compare against the positive limit of that sign.
    const long long limit = negative
        ? static_cast<long long>(std::numeric_limits<int>::max()) + 1
        : static_cast<long long>(std::numeric_limits<int>::max());
    long long value = 0;
    for (; pos < text.size(); ++pos) {
        const char ch = text[pos];
        // Any non-digit, including whitespace, a decimal point or a trailing character,
        // invalidates the whole text.
        if (ch < '0' || ch > '9') return false;
        const int digit = ch - '0';
        // Reject before multiplying so the accumulation can never overflow.
        if (value > (limit - digit) / 10) return false;
        value = value * 10 + digit;
    }
    // Only a fully validated result reaches the caller's variable.
    output = static_cast<int>(negative ? -value : value);
    return true;
}
// END IMPLEMENTATION
}

#define CHECK(...) do { if (!(__VA_ARGS__)) throw std::runtime_error("check failed"); } while(false)
#define EXPECT_INVALID(...) do { bool caught=false; try { (void)(__VA_ARGS__); } catch(const std::invalid_argument&) { caught=true; } CHECK(caught); } while(false)
#define EXPECT_RANGE(...) do { bool caught=false; try { (void)(__VA_ARGS__); } catch(const std::out_of_range&) { caught=true; } CHECK(caught); } while(false)

using namespace aim::b1;
int main() {
    try {
        { int x=9; CHECK(parse_i32("-12",x) && x==-12); }
        { int x=9; CHECK(!parse_i32("12x",x) && x==9); }
        std::cout << "PUBLIC 2/2\n";
    } catch(const std::exception& e) {
        std::cerr << e.what() << "\n";
        return 1;
    }
    return 0;
}
