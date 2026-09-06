#include <vector>
#include <algorithm>
#include <cmath>

class Solution {
public:
    int maximumGap(std::vector<int>& nums) {
        int n = nums.size();
        if (n < 2) return 0;
        int min_val = *std::min_element(nums.begin(), nums.end());
        int max_val = *std::max_element(nums.begin(), nums.end());
        if (min_val == max_val) return 0;

        int bucket_size = std::max(1, (max_val - min_val) / (n - 1));
        int num_buckets = (max_val - min_val) / bucket_size + 1;

        std::vector<int> min_b(num_buckets, INT_MAX);
        std::vector<int> max_b(num_buckets, -1);

        for (int x : nums) {
            int idx = (x - min_val) / bucket_size;
            min_b[idx] = std::min(min_b[idx], x);
            max_b[idx] = std::max(max_b[idx], x);
        }

        int max_gap = 0, prev_max = min_val;
        for (int i = 0; i < num_buckets; ++i) {
            if (max_b[i] == -1) continue;
            max_gap = std::max(max_gap, min_b[i] - prev_max);
            prev_max = max_b[i];
        }
        return max_gap;
    }
};
