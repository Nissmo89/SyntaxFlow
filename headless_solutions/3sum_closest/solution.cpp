#include <vector>
#include <algorithm> // Required for std::sort and std::abs
#include <limits>    // Required for std::numeric_limits (though not strictly needed with initial sum)

class Solution {
public:
    int threeSumClosest(std::vector<int>& nums, int target) {
        // Sort the array to enable the two-pointer approach.
        std::sort(nums.begin(), nums.end());
        int n = nums.size();
        
        // Initialize closest_sum with the sum of the first three elements.
        // Since n >= 3, this is always a valid initial sum.
        int closest_sum = nums[0] + nums[1] + nums[2];
        
        // Iterate through the array, fixing the first element of the triplet.
        // We go up to n - 2 because we need at least two more elements (left and right).
        for (int i = 0; i < n - 2; ++i) {
            int left = i + 1; // Pointer for the second element
            int right = n - 1; // Pointer for the third element
            
            // Use two pointers to find the remaining two elements
            while (left < right) {
                int current_sum = nums[i] + nums[left] + nums[right];
                
                // If the current sum is exactly the target, we found the closest possible sum.
                if (current_sum == target) {
                    return target;
                }
                
                // Update closest_sum if the current sum is closer to target
                // We compare absolute differences to find which sum is closer.
                if (std::abs(current_sum - target) < std::abs(closest_sum - target)) {
                    closest_sum = current_sum;
                }
                
                // Adjust pointers based on whether the current sum is too small or too large
                if (current_sum < target) {
                    // Sum is too small, need to increase it. Move left pointer forward.
                    left++;
                } else { // current_sum > target
                    // Sum is too large, need to decrease it. Move right pointer backward.
                    right--;
                }
            }
        }
        
        return closest_sum;
    }
};
