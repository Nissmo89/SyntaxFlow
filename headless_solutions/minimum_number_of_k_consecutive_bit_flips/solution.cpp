#include <vector>

class Solution {
public:
    int minKBitFlips(std::vector<int>& nums, int k) {
        int n = nums.size();
        int flipped = 0; // Tracks the flip parity of the current window
        int res = 0;     // Total number of k-bit flips

        for (int i = 0; i < n; ++i) {
            // Expire flip that started k steps ago
            if (i >= k && nums[i - k] >= 2) {
                flipped ^= 1;
            }

            // If effective bit is 0, we must initiate a flip at index i
            if (flipped == nums[i]) {
                if (i + k > n) {
                    return -1; // Out of bounds, impossible to flip k elements
                }
                nums[i] ^= 2; // Mark that a flip started at index i
                flipped ^= 1; // Increment current window flip state
                res++;
            }
        }

        return res;
    }
};
