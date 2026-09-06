#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>

class Solution {
public:
    int maxEqualRowsAfterFlips(std::vector<std::vector<int>>& matrix) {
        std::unordered_map<std::string, int> counts;
        for (const auto& row : matrix) {
            std::string pattern = "";
            int first = row[0];
            for (int val : row) {
                // Normalize: if first bit is 1, flip everything to start with 0
                pattern += std::to_string(val ^ first);
            }
            counts[pattern]++;
        }
        int max_rows = 0;
        for (auto const& [key, val] : counts) {
            max_rows = std::max(max_rows, val);
        }
        return max_rows;
    }
};
