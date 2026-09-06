class Solution {
public:
    int pivotIndex(vector<int>& nums) {
        int prefixSum = 0;
        for (int num : nums) {
            prefixSum += num;
        }
        
        int leftSum = 0;
        for (int i = 0; i < nums.size(); i++) {
            if (leftSum == prefixSum - leftSum - nums[i]) {
                return i;
            }
            leftSum += nums[i];
        }
        
        return -1;
    }
};
