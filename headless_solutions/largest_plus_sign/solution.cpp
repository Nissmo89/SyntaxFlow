class Solution {
public:
    int orderOfLargestPlusSign(int n, vector<vector<int>>& mines) {
        vector<vector<int>> grid(n, vector<int>(n, n));
        for (auto& m : mines) grid[m[0]][m[1]] = 0;
        
        for (int i = 0; i < n; ++i) {
            int l = 0, r = 0, u = 0, d = 0;
            for (int j = 0; j < n; ++j) {
                l = (grid[i][j] == 0) ? 0 : l + 1;
                grid[i][j] = min(grid[i][j], l);
                
                r = (grid[i][n - 1 - j] == 0) ? 0 : r + 1;
                grid[i][n - 1 - j] = min(grid[i][n - 1 - j], r);
                
                u = (grid[j][i] == 0) ? 0 : u + 1;
                grid[j][i] = min(grid[j][i], u);
                
                d = (grid[n - 1 - j][i] == 0) ? 0 : d + 1;
                grid[n - 1 - j][i] = min(grid[n - 1 - j][i], d);
            }
        }
        
        int res = 0;
        for (int i = 0; i < n; ++i)
            for (int j = 0; j < n; ++j)
                res = max(res, grid[i][j]);
        return res;
    }
};
