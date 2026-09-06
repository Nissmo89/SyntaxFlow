class Solution {
public:
    int maxRotateFunction(vector<int>& nums) {
        long long n = nums.size();
        long long sum = 0;
        long long f = 0;
        for (int i = 0; i < n; ++i) {
            sum += nums[i];
            f += (long long)i * nums[i];
        }
        long long max_f = f;
        for (int i = 1; i < n; ++i) {
            f = f + sum - n * nums[n - i];
            max_f = max(max_f, f);
        }
        return (int)max_f;
    }
};
