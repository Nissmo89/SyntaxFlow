class Solution {
public:
    int search(vector<int>& nums, int target) {
        int left = 0;
        int right = nums.size() - 1; // Initialize right pointer to the last index

        // Continue searching as long as the search space [left, right] is valid
        while (left <= right) {
            // Calculate the middle index to prevent potential integer overflow
            // (left + right) / 2 is fine for these constraints, but this is safer practice
            int mid = left + (right - left) / 2;

            if (nums[mid] == target) {
                // Target found, return its index
                return mid;
            } else if (nums[mid] < target) {
                // If middle element is less than target,
                // target must be in the right half.
                // Discard mid and everything to its left.
                left = mid + 1;
            } else { // nums[mid] > target
                // If middle element is greater than target,
                // target must be in the left half.
                // Discard mid and everything to its right.
                right = mid - 1;
            }
        }

        // If the loop finishes, target was not found in the array
        return -1;
    }
};
