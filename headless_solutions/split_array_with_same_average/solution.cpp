#include <vector>
#include <numeric>
#include <unordered_set>

class Solution {
public:
    bool splitArraySameAverage(std::vector<int>& nums) {
        int n = nums.size(), S = std::accumulate(nums.begin(), nums.end(), 0);
        int m = n / 2;
        std::vector<std::unordered_set<int>> left(m + 1);
        left[0].insert(0);
        for (int i = 0; i < m; ++i) {
            for (int j = i; j >= 0; --j) {
                for (int s : left[j]) left[j + 1].insert(s + nums[i]);
            }
        }
        std::vector<std::unordered_set<int>> right(n - m + 1);
        right[0].insert(0);
        for (int i = m; i < n; ++i) {
            for (int j = i - m; j >= 0; --j) {
                for (int s : right[j]) right[j + 1].insert(s + nums[i]);
            }
        }
        for (int k = 1; k <= n / 2; ++k) {
            if ((S * k) % n == 0) {
                int target = (S * k) / n;
                for (int i = 0; i <= k; ++i) {
                    if (i <= m && (k - i) <= (n - m)) {
                        if (left[i].count(target - 0) && right[k - i].count(target - *left[i].begin())) {
                            // This logic is simplified; check intersection of sums
                            for (int s : left[i]) if (right[k - i].count(target - s)) return true;
                        }
                    }
                }
            }
        }
        return false;
    }
};
