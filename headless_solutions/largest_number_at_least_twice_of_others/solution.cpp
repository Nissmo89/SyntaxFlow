class Solution {
public:
    int dominantIndex(vector<int>& nums) {
        int max = INT_MIN, maxIndex = -1;
        for (int i = 0; i < nums.size(); i++) {
            if (nums[i] > max) {
                max = nums[i];
                maxIndex = i;
            }
        }
        for (int i = 0; i < nums.size(); i++) {
            if (i != maxIndex && nums[i] > 0 && nums[i] * 2 > max) {
                return -1;
            }
        }
        return maxIndex;
    }
};
