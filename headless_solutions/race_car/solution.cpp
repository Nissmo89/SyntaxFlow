#include <vector>
#include <algorithm>
#include <cmath>

class Solution {
    int dp[10001] = {0};
public:
    int racecar(int target) {
        if (dp[target] > 0) return dp[target];
        int n = floor(log2(target)) + 1;
        if ((1 << n) - 1 == target) dp[target] = n;
        else {
            dp[target] = racecar((1 << n) - 1 - target) + n + 1;
            for (int m = 0; m < n - 1; ++m) {
                dp[target] = std::min(dp[target], racecar(target - (1 << (n - 1)) + (1 << m)) + n + m + 1);
            }
        }
        return dp[target];
    }
};
