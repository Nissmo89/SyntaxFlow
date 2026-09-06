class Solution {
public:
    int minMoves(vector<int>& nums) {
        long long sum = 0;
        int min_val = nums[0];
        for (int x : nums) {
            sum += x;
            if (x < min_val) min_val = x;
        }
        return (int)(sum - (long long)min_val * nums.size());
    }
};
