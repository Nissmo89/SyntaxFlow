class Solution {
public:
    int arrangeCoins(int n) {
        // Binary search approach
        long long low = 1; // Use long long for low to match high and mid types
        long long high = n; // Max possible k can be n, but will be much smaller (sqrt(2n))
        long long ans = 0;

        while (low <= high) {
            long long mid = low + (high - low) / 2;
            // Calculate coins needed for 'mid' complete rows
            // Use long long for current_k_coins to prevent overflow,
            // as mid * (mid + 1) can exceed INT_MAX for large n.
            long long current_k_coins = mid * (mid + 1) / 2;

            if (current_k_coins <= n) {
                // 'mid' rows are possible, try to find more complete rows
                ans = mid;
                low = mid + 1;
            } else {
                // 'mid' rows require too many coins, reduce the number of rows
                high = mid - 1;
            }
        }

        return (int)ans; // Cast back to int as the problem expects int return
    }
};
