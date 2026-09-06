class Solution {
public:
    int maxSumTwoNoOverlap(vector<int>& nums, int firstLen, int secondLen) {
        int n = nums.size();
        vector<int> prefixSum(n + 1, 0);
        for (int i = 0; i < n; ++i) prefixSum[i + 1] = prefixSum[i] + nums[i];
        
        auto solve = [&](int L, int M) {
            int res = 0, maxL = 0;
            for (int i = L + M; i <= n; ++i) {
                maxL = max(maxL, prefixSum[i - M] - prefixSum[i - M - L]);
                res = max(res, maxL + prefixSum[i] - prefixSum[i - M]);
            }
            return res;
        };
        return max(solve(firstLen, secondLen), solve(secondLen, firstLen));
    }
};
