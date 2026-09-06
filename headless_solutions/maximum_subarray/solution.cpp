#include <vector>
#include <algorithm>

class Solution {
public:
    int maxSubArray(std::vector<int>& nums) {
        int current_sum = nums[0];
        int global_max = nums[0];
        
        for (size_t i = 1; i < nums.size(); ++i) {
            // Either start a new subarray at i, or extend the existing one
            current_sum = std::max(nums[i], current_sum + nums[i]);
            global_max = std::max(global_max, current_sum);
        }
        
        return global_max;
    }
};
