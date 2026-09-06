/**
 * @param {number} n
 * @return {number}
 */
var arrangeCoins = function(n) {
    // Binary search approach
    let low = 1;
    let high = n;
    let ans = 0;

    while (low <= high) {
        let mid = Math.floor(low + (high - low) / 2);
        // Calculate coins needed for 'mid' complete rows
        // JavaScript numbers are 64-bit floating-point, but can represent
        // integers up to 2^53 - 1 precisely.
        // n is up to 2^31 - 1, so mid will be up to ~2^16.
        // mid * (mid + 1) will be up to ~2^32, which fits within
        // JavaScript's safe integer range (2^53 - 1).
        let current_k_coins = mid * (mid + 1) / 2;

        if (current_k_coins <= n) {
            // 'mid' rows are possible, try to find more complete rows
            ans = mid;
            low = mid + 1;
        } else {
            // 'mid' rows require too many coins, reduce the number of rows
            high = mid - 1;
        }
    }

    return ans;
};
