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

// Assessment 01 Q5; see 题面.pdf.
namespace aim::q5 {


// BEGIN IMPLEMENTATION
namespace {

// Only these two symbols may appear in a row: free space and obstacle.
bool is_map_symbol(char cell) {
    return cell == '.' || cell == '#';
}

}  // namespace

// Validate the whole map before any risk value is produced. A bad character in a
// later row still rejects the entire map, so no partial matrix is ever returned.
void validate_grid(const std::vector<std::string>& grid) {
    // An empty row sequence is a valid, empty map and needs no further checks.
    if (grid.empty()) return;
    const std::size_t columns = grid.front().size();
    // A nonempty map must have at least one column.
    if (columns == 0) throw std::invalid_argument("map has no columns");
    for (std::size_t row = 0; row < grid.size(); ++row) {
        // Every row must have exactly the same length; no padding or truncation.
        if (grid[row].size() != columns) {
            throw std::invalid_argument("rows have different lengths");
        }
        for (std::size_t column = 0; column < columns; ++column) {
            if (!is_map_symbol(grid[row][column])) {
                throw std::invalid_argument("map contains an illegal character");
            }
        }
    }
}

std::vector<std::vector<int>> risk(const std::vector<std::string>& grid) {
    validate_grid(grid);
    // Part 1 stops here: the neighbourhood risk matrix is still to be computed.
    return std::vector<std::vector<int>>();
}
// END IMPLEMENTATION
}

#define CHECK(...) do { if (!(__VA_ARGS__)) throw std::runtime_error("check failed"); } while(false)
#define EXPECT_INVALID(...) do { bool caught=false; try { (void)(__VA_ARGS__); } catch(const std::invalid_argument&) { caught=true; } CHECK(caught); } while(false)
#define EXPECT_RANGE(...) do { bool caught=false; try { (void)(__VA_ARGS__); } catch(const std::out_of_range&) { caught=true; } CHECK(caught); } while(false)

using namespace aim::q5;
int main() {
    try {
        { CHECK((risk({".#",".."})==std::vector<std::vector<int>>{{1,-1},{0,1}})); }
        { CHECK(risk({}).empty()); }
        std::cout << "PUBLIC 2/2\n";
    } catch(const std::exception& e) {
        std::cerr << e.what() << "\n";
        return 1;
    }
    return 0;
}
