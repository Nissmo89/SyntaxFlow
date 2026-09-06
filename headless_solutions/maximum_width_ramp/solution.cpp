class Solution {
public:
    int maxWidthRamp(vector<int>& nums) {
        int n = nums.size();
        vector<int> stack;
        for (int i = 0; i < n; ++i) {
            if (stack.empty() || nums[stack.back()] > nums[i]) {
                stack.push_back(i);
            }
        }
        int maxWidth = 0;
        for (int j = n - 1; j >= 0; --j) {
            while (!stack.empty() && nums[stack.back()] <= nums[j]) {
                maxWidth = max(maxWidth, j - stack.back());
                stack.pop_back();
            }
        }
        return maxWidth;
    }
};
