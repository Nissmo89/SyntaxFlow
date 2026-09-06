#include <vector>
#include <unordered_set>
#include <algorithm>

class Solution {
public:
    int flipgame(std::vector<int>& fronts, std::vector<int>& backs) {
        std::unordered_set<int> fixed_numbers;
        int n = fronts.size();
        
        // Step 1: Identify numbers that are always visible
        for (int i = 0; i < n; ++i) {
            if (fronts[i] == backs[i]) {
                fixed_numbers.insert(fronts[i]);
            }
        }
        
        int min_good = std::numeric_limits<int>::max();
        bool found = false;
        
        // Step 2: Check all numbers in fronts
        for (int val : fronts) {
            if (fixed_numbers.find(val) == fixed_numbers.end()) {
                min_good = std::min(min_good, val);
                found = true;
            }
        }
        
        // Step 3: Check all numbers in backs
        for (int val : backs) {
            if (fixed_numbers.find(val) == fixed_numbers.end()) {
                min_good = std::min(min_good, val);
                found = true;
            }
        }
        
        return found? min_good : 0;
    }
};
