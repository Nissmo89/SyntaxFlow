class Solution {
    int atMostK(vector<int>& nums, int k) {
        int n = nums.size(), left = 0, res = 0;
        unordered_map<int, int> count;
        for (int right = 0; right < n; ++right) {
            if (++count[nums[right]] == 1) k--;
            while (k < 0) {
                if (--count[nums[left]] == 0) k++;
                left++;
            }
            res += (right - left + 1);
        }
        return res;
    }
public:
    int subarraysWithKDistinct(vector<int>& nums, int k) {
        return atMostK(nums, k) - atMostK(nums, k - 1);
    }
};
