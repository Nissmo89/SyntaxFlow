class Solution {
public:
    int subarraysDivByK(vector<int>& nums, int k) {
        vector<int> count(k, 0);
        count[0] = 1;
        int prefix_sum = 0, result = 0;
        for (int x : nums) {
            prefix_sum = (prefix_sum + x) % k;
            if (prefix_sum < 0) prefix_sum += k;
            result += count[prefix_sum];
            count[prefix_sum]++;
        }
        return result;
    }
};
