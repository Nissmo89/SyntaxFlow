class Solution {
public:
    int numFactoredBinaryTrees(vector<int>& arr) {
        int mod = 1e9 + 7;
        unordered_map<int, long> dp;
        sort(arr.begin(), arr.end());
        for (int i = 0; i < arr.size(); i++) {
            dp[arr[i]] = 1;
            for (int j = 0; j < i; j++) {
                if (arr[i] % arr[j] == 0) {
                    int k = arr[i] / arr[j];
                    if (dp.find(k) != dp.end()) {
                        dp[arr[i]] = (dp[arr[i]] + dp[arr[j]] * dp[k]) % mod;
                    }
                }
            }
        }
        long res = 0;
        for (auto& pair : dp) {
            res = (res + pair.second) % mod;
        }
        return res;
    }
};
