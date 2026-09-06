#include <vector>
#include <numeric>
#include <algorithm>

class Solution {
public:
    int mergeStones(std::vector<int>& stones, int k) {
        int n = stones.size();
        // Check feasibility: each step reduces total piles by (k - 1)
        if ((n - 1) % (k - 1) != 0) {
            return -1;
        }

        // Prefix sum array for O(1) range sum queries
        std::vector<int> prefix(n + 1, 0);
        for (int i = 0; i < n; ++i) {
            prefix[i + 1] = prefix[i] + stones[i];
        }

        // dp[i][j] stores the min cost to merge stones[i...j] as much as possible
        std::vector<std::vector<int>> dp(n, std::vector<int>(n, 0));

        // Iterate over interval length from 2 to n
        for (int len = 2; len <= n; ++len) {
            for (int i = 0; i <= n - len; ++i) {
                int j = i + len - 1;
                dp[i][j] = 1e9; // Large constant representing infinity

                // mid advances by (k - 1) to ensure left part reduces to 1 pile
                for (int mid = i; mid < j; mid += k - 1) {
                    dp[i][j] = std::min(dp[i][j], dp[i][mid] + dp[mid + 1][j]);
                }

                // If this range can be merged into 1 pile, add total range sum cost
                if ((len - 1) % (k - 1) == 0) {
                    dp[i][j] += prefix[j + 1] - prefix[i];
                }
            }
        }

        return dp[0][n - 1];
    }
};
