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

// Assessment 01 Q4; see 题面.pdf.
namespace aim::q4 {
struct CommandResult { long long value; int accepted; int rejected; };

// BEGIN IMPLEMENTATION
CommandResult execute(const std::vector<std::string>& lines) {
    CommandResult result{0, 0, 0};
    // Both the argument and the proposed result share this inclusive bound.
    const long long kLimit = 1000000;
    for (const std::string& line : lines) {
        std::istringstream input(line);
        std::string operation;
        std::string extra;
        long long argument = 0;
        // The candidate is staged here and committed only after full validation.
        long long candidate = result.value;
        bool valid = false;
        if (input >> operation) {
            if (operation == "RESET") {
                // RESET takes no argument, so any remaining field rejects the line.
                if (!(input >> extra)) {
                    candidate = 0;
                    valid = true;
                }
            } else if (operation == "ADD" || operation == "SET") {
                // Exactly one argument, no trailing field, and the argument itself
                // must already lie inside the range before any arithmetic happens.
                if (input >> argument && !(input >> extra)
                    && argument >= -kLimit && argument <= kLimit) {
                    candidate = operation == "ADD" ? result.value + argument : argument;
                    // The proposed result must also stay inside the same range.
                    valid = candidate >= -kLimit && candidate <= kLimit;
                }
            }
        }
        // Increment the counter that belongs to this line, then keep the invariant
        // accepted+rejected == lines.size().
        if (valid) {
            result.value = candidate;
            ++result.accepted;
        } else {
            ++result.rejected;
        }
    }
    return result;
}
// END IMPLEMENTATION
}

#define CHECK(...) do { if (!(__VA_ARGS__)) throw std::runtime_error("check failed"); } while(false)
#define EXPECT_INVALID(...) do { bool caught=false; try { (void)(__VA_ARGS__); } catch(const std::invalid_argument&) { caught=true; } CHECK(caught); } while(false)
#define EXPECT_RANGE(...) do { bool caught=false; try { (void)(__VA_ARGS__); } catch(const std::out_of_range&) { caught=true; } CHECK(caught); } while(false)

using namespace aim::q4;
int main() {
    try {
        { aim::q4::CommandResult r=execute({"ADD 5","SET 8 x","RESET","ADD -2"}); CHECK(r.value==-2 && r.accepted==3 && r.rejected==1); }
        { aim::q4::CommandResult r=execute({}); CHECK(r.value==0 && r.accepted==0 && r.rejected==0); }
        std::cout << "PUBLIC 2/2\n";
    } catch(const std::exception& e) {
        std::cerr << e.what() << "\n";
        return 1;
    }
    return 0;
}
