class Solution {
public:
    int findPaths(int m, int n, int maxMove, int startRow, int startColumn) {
        int MOD = 1e9 + 7;
        vector<vector<int>> dp(m, vector<int>(n, 0));
        dp[startRow][startColumn] = 1;
        int count = 0;
        int dirs[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

        for (int move = 0; move < maxMove; ++move) {
            vector<vector<int>> next_dp(m, vector<int>(n, 0));
            for (int r = 0; r < m; ++r) {
                for (int c = 0; c < n; ++c) {
                    if (dp[r][c] == 0) continue;
                    for (auto& d : dirs) {
                        int nr = r + d[0], nc = c + d[1];
                        if (nr < 0 || nr >= m || nc < 0 || nc >= n) {
                            count = (count + dp[r][c]) % MOD;
                        } else {
                            next_dp[nr][nc] = (next_dp[nr][nc] + dp[r][c]) % MOD;
                        }
                    }
                }
            }
            dp = next_dp;
        }
        return count;
    }
};
