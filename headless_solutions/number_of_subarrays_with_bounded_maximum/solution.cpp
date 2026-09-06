class Solution {
public:
    int numSubarrayBoundedMax(vector<int>& nums, int left, int right) {
        int res = 0, last_invalid = -1, last_candidate = -1;
        for (int i = 0; i < nums.size(); ++i) {
            if (nums[i] > right) last_invalid = i;
            if (nums[i] >= left) last_candidate = i;
            res += (last_candidate - last_invalid);
        }
        return res;
    }
};
