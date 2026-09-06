#include <vector>
#include <algorithm> // Required for std::sort
#include <numeric>   // Not strictly needed for this solution, but useful for other array problems

class Solution {
public:
    int arrayPairSum(std::vector<int>& nums) {
        // Step 1: Sort the array in non-decreasing order.
        // This ensures that adjacent elements are as close as possible,
        // minimizing the "loss" from the larger element in each pair.
        std::sort(nums.begin(), nums.end());

        int maximized_sum = 0;
        // Step 2: Iterate through the sorted array and sum every second element.
        // These elements (at indices 0, 2, 4, ...) will always be the minimum
        // of their respective pairs (nums[i], nums[i+1]).
        for (int i = 0; i < nums.size(); i += 2) {
            maximized_sum += nums[i];
        }

        return maximized_sum;
    }
};
