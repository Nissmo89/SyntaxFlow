/**
 * @param {number[]} prices
 * @return {number}
 */
var maxProfit = function(prices) {
    let min_price = prices[0];
    let max_profit = 0;
    for (let i = 1; i < prices.length; i++) {
        if (prices[i] < min_price) {
            min_price = prices[i];
        } else {
            max_profit = Math.max(max_profit, prices[i] - min_price);
        }
    }
    return max_profit;
};
