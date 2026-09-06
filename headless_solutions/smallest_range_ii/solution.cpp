#include <vector>
#include <algorithm>

class Solution {
public:
    int smallestRangeII(std::vector<int>& nums, int k) {
        std::sort(nums.begin(), nums.end());
        int n = nums.size();
        int res = nums[n - 1] - nums[0];
        
        for (int i = 0; i < n - 1; ++i) {
            int high = std::max(nums[n - 1] - k, nums[i] + k);
            int low = std::min(nums[0] + k, nums[i + 1] - k);
            res = std::min(res, high - low);
        }
        return res;
    }
};
