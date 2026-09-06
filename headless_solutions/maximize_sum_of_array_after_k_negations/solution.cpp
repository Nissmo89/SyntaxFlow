class Solution {
public:
    int largestSumAfterKNegations(vector<int>& nums, int k) {
        sort(nums.begin(), nums.end());
        for (int i = 0; k > 0 && i < nums.size() && nums[i] < 0; ++i, --k)
            nums[i] = -nums[i];
        int res = 0, min = INT_MAX;
        for (int a : nums) {
            res += a;
            min = min(min, a);
        }
        return res - (k % 2) * min * 2;
    }
};
