#include <vector>
#include <unordered_map>

class Solution {
public:
    int subarraySum(std::vector<int>& nums, int k) {
        std::unordered_map<int, int> prefix_sums;
        prefix_sums[0] = 1; // Base case: prefix sum of 0 exists once
        int count = 0, current_sum = 0;
        
        for (int num : nums) {
            current_sum += num;
            if (prefix_sums.find(current_sum - k) != prefix_sums.end()) {
                count += prefix_sums[current_sum - k];
            }
            prefix_sums[current_sum]++;
        }
        return count;
    }
};
