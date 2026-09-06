#include <vector>
#include <numeric>
#include <cmath>

class Solution {
public:
    int findTargetSumWays(std::vector<int>& nums, int target) {
        int sum = std::accumulate(nums.begin(), nums.end(), 0);
        if (std::abs(target) > sum || (target + sum) % 2 != 0) return 0;
        
        int subset_target = (target + sum) / 2;
        std::vector<int> dp(subset_target + 1, 0);
        dp[0] = 1;
        
        for (int n : nums) {
            for (int i = subset_target; i >= n; --i) {
                dp[i] += dp[i - n];
            }
        }
        return dp[subset_target];
    }
};
