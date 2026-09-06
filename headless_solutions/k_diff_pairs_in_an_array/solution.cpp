#include <vector>
#include <unordered_map>

class Solution {
public:
    int findPairs(std::vector<int>& nums, int k) {
        if (k < 0) return 0;
        std::unordered_map<int, int> counts;
        for (int num : nums) counts[num]++;
        
        int pairs = 0;
        for (auto const& [val, count] : counts) {
            if (k == 0) {
                if (count > 1) pairs++;
            } else {
                if (counts.count(val + k)) pairs++;
            }
        }
        return pairs;
    }
};
