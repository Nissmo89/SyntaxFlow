class Solution {
public:
    int findNumberOfLIS(vector<int>& nums) {
        int n = nums.size();
        if (n <= 1) return n;
        vector<int> lengths(n, 1), counts(n, 1);
        int maxLen = 1;
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < i; ++j) {
                if (nums[i] > nums[j]) {
                    if (lengths[j] + 1 > lengths[i]) {
                        lengths[i] = lengths[j] + 1;
                        counts[i] = counts[j];
                    } else if (lengths[j] + 1 == lengths[i]) {
                        counts[i] += counts[j];
                    }
                }
            }
            maxLen = max(maxLen, lengths[i]);
        }
        int result = 0;
        for (int i = 0; i < n; ++i) {
            if (lengths[i] == maxLen) result += counts[i];
        }
        return result;
    }
};
