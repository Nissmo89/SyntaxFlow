class Solution {
public:
    int thirdMax(vector<int>& nums) {
        sort(nums.begin(), nums.end());
        int elemCounted = 1;
        int prevElem = nums[0];
        for (int i = 1; i < nums.size(); i++) {
            if (nums[i] != prevElem) {
                elemCounted++;
                prevElem = nums[i];
                if (elemCounted == 3) {
                    return nums[i];
                }
            }
        }
        return nums[0];
    }
};
