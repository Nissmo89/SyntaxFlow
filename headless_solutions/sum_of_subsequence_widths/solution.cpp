#include <vector>
#include <algorithm>

class Solution {
public:
    int sumSubseqWidths(std::vector<int>& nums) {
        std::sort(nums.begin(), nums.end());
        long long mod = 1e9 + 7;
        long long res = 0;
        long long p2 = 1; // Represents 2^i
        int n = nums.size();
        
        for (int i = 0; i < n; ++i) {
            // Contribution: nums[i] * (2^i - 2^(n-1-i))
            res = (res + (long long)nums[i] * p2) % mod;
            res = (res - (long long)nums[n - 1 - i] * p2) % mod;
            p2 = (p2 * 2) % mod;
        }
        return (res + mod) % mod;
    }
};
