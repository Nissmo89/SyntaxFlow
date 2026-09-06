#include <vector>
#include <algorithm> // Required for std::sort

class Solution {
public:
    std::vector<std::vector<int>> threeSum(std::vector<int>& nums) {
        std::vector<std::vector<int>> result;
        int n = nums.size();

        // 1. Sort the array
        std::sort(nums.begin(), nums.end());

        // Iterate through the array to fix the first element (nums[i])
        // We iterate up to n-2 because we need at least two more elements (left, right)
        for (int i = 0; i < n - 2; ++i) {
            // Skip duplicate elements for nums[i]
            // If current element is same as previous, it will form duplicate triplets
            if (i > 0 && nums[i] == nums[i-1]) {
                continue;
            }

            // Optimization: If nums[i] is positive, sum cannot be zero with other non-negative numbers
            if (nums[i] > 0) {
                break;
            }

            // Initialize two pointers for the remaining part of the array
            int left = i + 1;
            int right = n - 1;
            int target = -nums[i]; // We need nums[left] + nums[right] to be this target

            while (left < right) {
                int current_sum = nums[left] + nums[right];

                if (current_sum == target) {
                    // Found a triplet
                    result.push_back({nums[i], nums[left], nums[right]});

                    // Skip duplicate elements for nums[left]
                    // Move left pointer past all elements equal to nums[left]
                    while (left < right && nums[left] == nums[left+1]) {
                        left++;
                    }
                    // Skip duplicate elements for nums[right]
                    // Move right pointer past all elements equal to nums[right]
                    while (left < right && nums[right] == nums[right-1]) {
                        right--;
                    }

                    // Move both pointers inwards to find new unique pairs
                    left++;
                    right--;
                } else if (current_sum < target) {
                    // Sum is too small, need a larger number from the left side
                    left++;
                } else { // current_sum > target
                    // Sum is too large, need a smaller number from the right side
                    right--;
                }
            }
        }

        return result;
    }
};
