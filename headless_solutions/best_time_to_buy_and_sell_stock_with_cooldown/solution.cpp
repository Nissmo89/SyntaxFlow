class Solution {
public:
    int maxProfit(vector<int>& prices) {
        int n = prices.size();
        vector<int> buy(n, 0), sell(n, 0), rest(n, 0);
        
        buy[0] = -prices[0];
        sell[0] = 0;
        rest[0] = 0;
        
        for (int i = 1; i < n; i++) {
            buy[i] = max(rest[i-1] - prices[i], buy[i-1]);
            sell[i] = max(buy[i-1] + prices[i], sell[i-1]);
            rest[i] = max(sell[i-1], buy[i-1], rest[i-1]);
        }
        
        return max(sell[n-1], rest[n-1]);
    }
};
