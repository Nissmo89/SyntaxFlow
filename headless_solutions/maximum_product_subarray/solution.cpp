class Solution {
public:
    int maxProduct(vector<int>& nums) {
        int globalMax = nums[0];
        int curMax = nums[0];
        int curMin = nums[0];
        
        for (size_t i = 1; i < nums.size(); ++i) {
            if (nums[i] < 0) swap(curMax, curMin);
            
            curMax = max(nums[i], curMax * nums[i]);
            curMin = min(nums[i], curMin * nums[i]);
            
            globalMax = max(globalMax, curMax);
        }
        return globalMax;
    }
};
