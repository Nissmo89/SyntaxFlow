class Solution {
    const int MOD = 1337;
    int myPow(int a, int k) {
        a %= MOD;
        int res = 1;
        for (int i = 0; i < k; ++i) res = (res * a) % MOD;
        return res;
    }
public:
    int superPow(int a, vector<int>& b) {
        int res = 1;
        for (int d : b) {
            res = (myPow(res, 10) * myPow(a, d)) % MOD;
        }
        return res;
    }
};
