#include <vector>
#include <cmath>
#include <algorithm>

class Solution {
public:
    bool judgePoint24(std::vector<int>& cards) {
        std::vector<double> nums(cards.begin(), cards.end());
        return solve(nums);
    }

private:
    bool solve(std::vector<double>& nums) {
        if (nums.size() == 1) return std::abs(nums[0] - 24.0) < 1e-6;
        for (int i = 0; i < nums.size(); ++i) {
            for (int j = 0; j < nums.size(); ++j) {
                if (i == j) continue;
                std::vector<double> next;
                for (int k = 0; k < nums.size(); ++k) {
                    if (k != i && k != j) next.push_back(nums[k]);
                }
                double a = nums[i], b = nums[j];
                std::vector<double> results = {a + b, a - b, a * b};
                if (std::abs(b) > 1e-6) results.push_back(a / b);
                for (double res : results) {
                    next.push_back(res);
                    if (solve(next)) return true;
                    next.pop_back();
                }
            }
        }
        return false;
    }
};
