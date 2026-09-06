// C++ implementation matching signature
class Solution {
public:
    double knightProbability(int n, int k, int row, int column) {
        // Directions a knight can move
        const int dr[8] = {-2,-1,1,2,2,1,-1,-2};
        const int dc[8] = {-1,-2,-2,-1,1,2,2,1};

        // dp and next are 2‑D arrays of size n × n
        vector<vector<double>> dp(n, vector<double>(n, 0.0));
        vector<vector<double>> next(n, vector<double>(n, 0.0));

        dp[row][column] = 1.0;          // start position

        for (int step = 0; step < k; ++step) {
            // reset next layer
            for (int r = 0; r < n; ++r)
                fill(next[r].begin(), next[r].end(), 0.0);

            for (int r = 0; r < n; ++r) {
                for (int c = 0; c < n; ++c) {
                    if (dp[r][c] == 0.0) continue;
                    double prob = dp[r][c] / 8.0;
                    for (int m = 0; m < 8; ++m) {
                        int nr = r + dr[m];
                        int nc = c + dc[m];
                        if (nr >= 0 && nr < n && nc >= 0 && nc < n)
                            next[nr][nc] += prob;
                    }
                }
            }
            dp.swap(next);
        }

        double ans = 0.0;
        for (int r = 0; r < n; ++r)
            for (int c = 0; c < n; ++c)
                ans += dp[r][c];
        return ans;
    }
};
