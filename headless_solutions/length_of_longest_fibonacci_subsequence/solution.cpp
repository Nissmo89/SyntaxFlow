class Solution {
public:
    int lenLongestFibSubseq(vector<int>& arr) {
        int n = arr.size();
        unordered_map<int, int> valToIndex;
        for (int i = 0; i < n; ++i) valToIndex[arr[i]] = i;
        
        vector<vector<int>> dp(n, vector<int>(n, 0));
        int maxLen = 0;
        
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < i; ++j) {
                int diff = arr[i] - arr[j];
                if (diff < arr[j] && valToIndex.count(diff)) {
                    int k = valToIndex[diff];
                    dp[j][i] = dp[k][j] + 1;
                    maxLen = max(maxLen, dp[j][i] + 2);
                }
            }
        }
        return maxLen >= 3 ? maxLen : 0;
    }
};
