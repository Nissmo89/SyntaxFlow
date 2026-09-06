#include <vector>
#include <algorithm>
#include <iostream>

class Solution {
public:
    // Helper function to count pairs with distance <= max_dist
    int countPairs(const std::vector<int>& nums, int max_dist) {
        int count = 0;
        int n = nums.size();
        int right = 0; // The right pointer for the sliding window

        // Iterate through each element as the 'left' element of a pair
        for (int left = 0; left < n; ++left) {
            // Move the 'right' pointer forward as long as the distance is within max_dist
            // and we haven't reached the end of the array.
            // Note: 'right' must be strictly greater than 'left' for a valid pair.
            // The condition `nums[right] - nums[left] <= max_dist` ensures we find
            // all elements `nums[right]` that form a valid pair with `nums[left]`.
            while (right < n && nums[right] - nums[left] <= max_dist) {
                right++;
            }
            // For the current `nums[left]`, all elements from `nums[left + 1]` up to
            // `nums[right - 1]` form pairs with distance <= max_dist.
            // The number of such elements is `(right - 1) - (left + 1) + 1 = right - 1 - left`.
            // Since `right` is already advanced to the first element *not* satisfying the condition,
            // the count of valid elements *after* `left` is `right - (left + 1)`.
            // This is equivalent to `right - 1 - left`.
            count += (right - 1 - left);
        }
        return count;
    }

    int smallestDistancePair(std::vector<int>& nums, int k) {
        // 1. Sort the array to enable efficient counting.
        std::sort(nums.begin(), nums.end());
        int n = nums.size();

        // 2. Define the search space for the distance.
        // The minimum possible distance is 0.
        // The maximum possible distance is the difference between the largest and smallest elements.
        int low = 0;
        int high = nums[n - 1] - nums[0];
        int ans = high; // Initialize answer to the maximum possible distance

        // 3. Binary search for the k-th smallest distance.
        while (low <= high) {
            int mid = low + (high - low) / 2; // Candidate distance

            // Count how many pairs have a distance less than or equal to 'mid'.
            int count = countPairs(nums, mid);

            // If the count is less than k, it means 'mid' is too small.
            // The k-th smallest distance must be larger.
            if (count < k) {
                low = mid + 1;
            } else {
                // If the count is k or more, 'mid' could be the answer,
                // or the answer could be a smaller distance.
                // We store 'mid' as a potential answer and try smaller distances.
                ans = mid; // 'mid' is a possible answer
                high = mid - 1;
            }
        }

        // The loop terminates when low > high. 'ans' holds the smallest distance 'd'
        // such that countPairs(nums, d) >= k.
        return ans;
    }
};
