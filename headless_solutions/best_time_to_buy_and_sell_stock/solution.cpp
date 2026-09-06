#include <vector>
#include <algorithm> // Required for std::min and std::max
#include <limits>    // Required for std::numeric_limits

class Solution {
public:
    int maxProfit(std::vector<int>& prices) {
        // Initialize min_price to a very large value.
        // This ensures the first price encountered will become the initial min_price.
        int min_price = std::numeric_limits<int>::max();
        // Initialize max_profit to 0, as no profit is made initially.
        // If no profitable transaction is possible, 0 will be returned.
        int max_profit = 0;

        // Iterate through each price in the array.
        for (int price : prices) {
            // Update min_price: keep track of the lowest price seen so far.
            min_price = std::min(min_price, price);
            
            // Calculate the potential profit if we buy at min_price and sell at the current price.
            // Update max_profit: keep track of the maximum profit found.
            max_profit = std::max(max_profit, price - min_price);
        }

        // Return the maximum profit found.
        return max_profit;
    }
};
