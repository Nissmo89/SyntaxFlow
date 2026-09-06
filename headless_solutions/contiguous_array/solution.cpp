#include <vector>
#include <unordered_map>
#include <algorithm>

class Solution {
public:
    int findMaxLength(std::vector<int>& nums) {
        // Map to store the first occurrence of each count value.
        // Key: count, Value: index
        std::unordered_map<int, int> prefix_sum_map;
        
        // Initialize the map with count 0 at index -1.
        // This handles subarrays starting from index 0.
        prefix_sum_map[0] = -1;
        
        int max_length = 0;
        int count = 0; // Running count: +1 for 1, -1 for 0
        
        for (int i = 0; i < nums.size(); ++i) {
            // Update count based on the current element
            if (nums[i] == 1) {
                count++;
            } else { // nums[i] == 0
                count--;
            }
            
            // Check if this count has been seen before
            if (prefix_sum_map.count(count)) {
                // If seen, calculate the length of the subarray
                // from the previous occurrence of this count to the current index.
                int prev_index = prefix_sum_map[count];
                max_length = std::max(max_length, i - prev_index);
            } else {
                // If not seen, store the current index as the first occurrence
                prefix_sum_map[count] = i;
            }
        }
        
        return max_length;
    }
};
