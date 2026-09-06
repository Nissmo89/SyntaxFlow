class Solution {
public:
    int cherryPickup(vector<vector<int>>& grid) {
        const int n = grid.size();
        const int INF = -1e9;               // represents unreachable state
        // dp[r1][r2] for the current step t
        vector<vector<int>> prev(n, vector<int>(n, INF));
        vector<vector<int>> cur(n, vector<int>(n, INF));

        // step 0: both at (0,0)
        prev[0][0] = grid[0][0] == -1 ? INF : grid[0][0];

        // total steps needed to reach (n-1,n-1) from (0,0) moving only right/down
        for (int t = 1; t <= 2 * n - 2; ++t) {
            // reset current layer
            for (int i = 0; i < n; ++i)
                fill(cur[i].begin(), cur[i].end(), INF);

            // r1 ranges such that c1 = t - r1 is inside [0, n-1]
            int r1_min = max(0, t - (n - 1));
            int r1_max = min(n - 1, t);
            for (int r1 = r1_min; r1 <= r1_max; ++r1) {
                int c1 = t - r1;
                if (grid[r1][c1] == -1) continue;   // first person blocked

                int r2_min = max(0, t - (n - 1));
                int r2_max = min(n - 1, t);
                for (int r2 = r2_min; r2 <= r2_max; ++r2) {
                    int c2 = t - r2;
                    if (grid[r2][c2] == -1) continue; // second person blocked

                    int cherries = grid[r1][c1];
                    if (r1 != r2 || c1 != c2)   // avoid double‑count same cell
                        cherries += grid[r2][c2];

                    int best = INF;
                    // four possible previous moves
                    if (r1 > 0 && r2 > 0) best = max(best, prev[r1-1][r2-1]); // both down
                    if (r1 > 0)           best = max(best, prev[r1-1][r2]);   // p1 down, p2 right
                    if (r2 > 0)           best = max(best, prev[r1][r2-1]);   // p1 right, p2 down
                                         best = max(best, prev[r1][r2]);   // both right

                    if (best != INF)
                        cur[r1][r2] = best + cherries;
                }
            }
            swap(prev, cur);
        }

        int ans = prev[n-1][n-1];
        return ans < 0 ? 0 : ans;
    }
};
