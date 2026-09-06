class Solution {
public:
    bool checkPossibility(vector<int>& nums) {
        int count = 0;
        for (int i = 1; i < nums.size(); ++i) {
            if (nums[i - 1] > nums[i]) {
                if (++count > 1) return false;
                // If i < 2, we can always lower nums[i-1]
                // If nums[i-2] <= nums[i], we can lower nums[i-1] to nums[i]
                if (i < 2 || nums[i - 2] <= nums[i]) {
                    nums[i - 1] = nums[i];
                } else {
                    // Otherwise, we must raise nums[i] to nums[i-1]
                    nums[i] = nums[i - 1];
                }
            }
        }
        return true;
    }
};
