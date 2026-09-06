class Solution {
public:
    double champagneTower(int poured, int query_row, int query_glass) {
        // dp[r][c] = total champagne that has passed through glass (r,c)
        vector<vector<double>> dp(query_row + 2, vector<double>(query_row + 2, 0.0));
        dp[0][0] = static_cast<double>(poured);

        for (int r = 0; r <= query_row; ++r) {
            for (int c = 0; c <= r; ++c) {
                if (dp[r][c] > 1.0) {
                    double overflow = (dp[r][c] - 1.0) / 2.0;
                    dp[r + 1][c]     += overflow;   // left child
                    dp[r + 1][c + 1] += overflow;   // right child
                    dp[r][c] = 1.0;                  // glass is now full
                }
            }
        }
        return dp[query_row][query_glass];
    }
};
