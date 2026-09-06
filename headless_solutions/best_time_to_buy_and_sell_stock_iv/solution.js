/**
 * @param {number} k
 * @param {number[]} prices
 * @return {number}
 */
var maxProfit = function(k, prices) {
    const n = prices.length;
    if (n <= 1 || k === 0) return 0;

    // Optimization when k is large enough to allow unlimited transactions
    if (k >= Math.floor(n / 2)) {
        let maxProfit = 0;
        for (let i = 1; i < n; i++) {
            if (prices[i] > prices[i - 1]) {
                maxProfit += prices[i] - prices[i - 1];
            }
        }
        return maxProfit;
    }

    const hold = new Array(k + 1).fill(-Infinity);
    const sell = new Array(k + 1).fill(0);

    for (const price of prices) {
        for (let t = 1; t <= k; t++) {
            hold[t] = Math.max(hold[t], sell[t - 1] - price);
            sell[t] = Math.max(sell[t], hold[t] + price);
        }
    }

    return sell[k];
};
