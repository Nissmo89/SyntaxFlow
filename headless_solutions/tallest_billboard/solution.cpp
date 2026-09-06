#include <vector>
#include <algorithm>
#include <cmath>
#include <cstring>

class Solution {
public:
    int tallestBillboard(std::vector<int>& rods) {
        int sum = 0;
        for (int r : rods) sum += r;
        std::vector<int> dp(sum + 1, -1);
        dp[0] = 0;
        
        for (int r : rods) {
            std::vector<int> next_dp = dp;
            for (int d = 0; d <= sum - r; ++d) {
                if (dp[d] < 0) continue;
                // Add to taller
                next_dp[d + r] = std::max(next_dp[d + r], dp[d]);
                // Add to shorter
                int new_diff = std::abs(d - r);
                next_dp[new_diff] = std::max(next_dp[new_diff], dp[d] + std::min(d, r));
            }
            dp = next_dp;
        }
        return dp[0];
    }
};
