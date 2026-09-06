#include <vector>
#include <algorithm>
#include <climits>

class Solution {
public:
    int maxProfit(int k, std::vector<int>& prices) {
        int n = prices.size();
        if (n <= 1 || k == 0) return 0;

        // Optimization: If k >= n / 2, we can make as many transactions as we want (Greedy approach)
        if (k >= n / 2) {
            int max_profit = 0;
            for (int i = 1; i < n; ++i) {
                if (prices[i] > prices[i - 1]) {
                    max_profit += prices[i] - prices[i - 1];
                }
            }
            return max_profit;
        }

        // DP state arrays for 1 to k transactions
        // Use INT_MIN / 2 to prevent integer underflow during subtraction
        std::vector<int> hold(k + 1, INT_MIN / 2);
        std::vector<int> sell(k + 1, 0);

        for (int price : prices) {
            for (int t = 1; t <= k; ++t) {
                // Either keep holding or buy stock today using profit from (t-1)th sell
                hold[t] = std::max(hold[t], sell[t - 1] - price);
                // Either stay sold or sell stock today that was held in t-th transaction
                sell[t] = std::max(sell[t], hold[t] + price);
            }
        }

        return sell[k];
    }
};
