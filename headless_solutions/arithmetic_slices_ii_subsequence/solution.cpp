#include <vector>
#include <unordered_map>

class Solution {
public:
    int numberOfArithmeticSlices(std::vector<int>& nums) {
        int n = nums.size();
        int total = 0;
        std::vector<std::unordered_map<long long, int>> dp(n);
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < i; ++j) {
                long long diff = (long long)nums[i] - nums[j];
                int count_j = dp[j].count(diff) ? dp[j][diff] : 0;
                dp[i][diff] += count_j + 1;
                total += count_j;
            }
        }
        return total;
    }
};
