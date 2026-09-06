/**
 * @param {number[]} prices
 * @return {number}
 */
var maxProfit = function(prices) {
    let n = prices.length;
    let buy = new Array(n).fill(0);
    let sell = new Array(n).fill(0);
    let rest = new Array(n).fill(0);
    
    buy[0] = -prices[0];
    sell[0] = 0;
    rest[0] = 0;
    
    for (let i = 1; i < n; i++) {
        buy[i] = Math.max(rest[i-1] - prices[i], buy[i-1]);
        sell[i] = Math.max(buy[i-1] + prices[i], sell[i-1]);
        rest[i] = Math.max(sell[i-1], buy[i-1], rest[i-1]);
    }
    
    return Math.max(sell[n-1], rest[n-1]);
};
