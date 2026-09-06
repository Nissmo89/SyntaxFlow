#include <vector>
#include <numeric>

class Solution {
public:
    std::vector<int> maxSumOfThreeSubarrays(std::vector<int>& nums, int k) {
        int n = nums.size();
        int num_windows = n - k + 1;
        
        // Use 64-bit integers to prevent overflow during sum calculations
        std::vector<long long> W(num_windows, 0);
        long long current_sum = 0;
        
        for (int i = 0; i < k; ++i) {
            current_sum += nums[i];
        }
        W[0] = current_sum;
        
        for (int i = 1; i < num_windows; ++i) {
            current_sum += nums[i + k - 1] - nums[i - 1];
            W[i] = current_sum;
        }

        // left[i]: index of max window in W[0...i]
        std::vector<int> left(num_windows, 0);
        int best_left = 0;
        for (int i = 0; i < num_windows; ++i) {
            if (W[i] > W[best_left]) {
                best_left = i;
            }
            left[i] = best_left;
        }

        // right[i]: index of max window in W[i...num_windows-1]
        std::vector<int> right(num_windows, 0);
        int best_right = num_windows - 1;
        for (int i = num_windows - 1; i >= 0; --i) {
            if (W[i] >= W[best_right]) { // >= ensures smallest index chosen when searching backward
                best_right = i;
            }
            right[i] = best_right;
        }

        // Search for the optimal middle window
        long long max_sum = -1;
        std::vector<int> result(3, 0);

        for (int m = k; m <= n - 2 * k; ++m) {
            int l = left[m - k];
            int r = right[m + k];
            long long total = W[l] + W[m] + W[r];
            
            if (total > max_sum) {
                max_sum = total;
                result = {l, m, r};
            }
        }

        return result;
    }
};
