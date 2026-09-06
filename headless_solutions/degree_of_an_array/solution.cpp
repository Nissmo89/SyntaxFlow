#include <vector>
#include <unordered_map>
#include <algorithm> // For std::max and std::min

class Solution {
public:
    int findShortestSubArray(std::vector<int>& nums) {
        // Map to store the first occurrence index of each number
        std::unordered_map<int, int> left_idx;
        // Map to store the last occurrence index of each number
        std::unordered_map<int, int> right_idx;
        // Map to store the frequency (count) of each number
        std::unordered_map<int, int> counts;

        // First pass: Populate the maps
        for (int i = 0; i < nums.size(); ++i) {
            int num = nums[i];
            
            // If num is not in left_idx, this is its first occurrence
            if (left_idx.find(num) == left_idx.end()) {
                left_idx[num] = i;
            }
            
            // Always update right_idx to store the latest (last) occurrence
            right_idx[num] = i;
            
            // Increment the count for num
            counts[num]++;
        }

        // Find the maximum frequency (degree of the array)
        int degree = 0;
        for (auto const& [num, count] : counts) {
            degree = std::max(degree, count);
        }

        // Initialize min_len with the maximum possible length (length of nums itself)
        int min_len = nums.size();

        // Second pass: Find the shortest subarray with the same degree
        for (auto const& [num, count] : counts) {
            // If this number's frequency matches the array's degree
            if (count == degree) {
                // Calculate the length of the subarray containing all occurrences of num
                int current_len = right_idx[num] - left_idx[num] + 1;
                // Update min_len if this subarray is shorter
                min_len = std::min(min_len, current_len);
            }
        }

        return min_len;
    }
};
