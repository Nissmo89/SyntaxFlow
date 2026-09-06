class Solution {
    int res = 0, empty = 0;
    void dfs(vector<vector<int>>& grid, int x, int y, int count) {
        if (x < 0 || x >= grid.size() || y < 0 || y >= grid[0].size() || grid[x][y] == -1) return;
        if (grid[x][y] == 2) {
            if (count == empty) res++;
            return;
        }
        int temp = grid[x][y];
        grid[x][y] = -1;
        dfs(grid, x + 1, y, count + 1);
        dfs(grid, x - 1, y, count + 1);
        dfs(grid, x, y + 1, count + 1);
        dfs(grid, x, y - 1, count + 1);
        grid[x][y] = temp;
    }
public:
    int uniquePathsIII(vector<vector<int>>& grid) {
        int sx = 0, sy = 0;
        for (int i = 0; i < grid.size(); ++i) {
            for (int j = 0; j < grid[0].size(); ++j) {
                if (grid[i][j] == 1) { sx = i; sy = j; }
                else if (grid[i][j] == 0) empty++;
            }
        }
        dfs(grid, sx, sy, 0);
        return res;
    }
};
