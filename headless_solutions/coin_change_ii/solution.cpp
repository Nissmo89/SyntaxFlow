class Solution {
public:
    int change(int amount, vector<int>& coins) {
        // dp[i] = number of ways to make amount i
        vector<int> dp(amount + 1, 0);
        dp[0] = 1; // one way to make amount 0: use no coins

        for (int coin : coins) {
            // iterate amounts in increasing order to allow unlimited use of this coin
            for (int amt = coin; amt <= amount; ++amt) {
                dp[amt] += dp[amt - coin];
            }
        }

        return dp[amount];
    }
};
