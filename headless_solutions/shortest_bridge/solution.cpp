class Solution {
    int dirs[4][2] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};
    void dfs(vector<vector<int>>& grid, int r, int c, queue<pair<int, int>>& q) {
        if (r < 0 || c < 0 || r >= grid.size() || c >= grid[0].size() || grid[r][c] != 1) return;
        grid[r][c] = 2;
        q.push({r, c});
        for (auto& d : dirs) dfs(grid, r + d[0], c + d[1], q);
    }
public:
    int shortestBridge(vector<vector<int>>& grid) {
        int n = grid.size();
        queue<pair<int, int>> q;
        bool found = false;
        for (int i = 0; i < n && !found; ++i)
            for (int j = 0; j < n && !found; ++j)
                if (grid[i][j] == 1) { dfs(grid, i, j, q); found = true; }
        
        int dist = 0;
        while (!q.empty()) {
            int size = q.size();
            while (size--) {
                auto [r, c] = q.front(); q.pop();
                for (auto& d : dirs) {
                    int nr = r + d[0], nc = c + d[1];
                    if (nr >= 0 && nc >= 0 && nr < n && nc < n) {
                        if (grid[nr][nc] == 1) return dist;
                        if (grid[nr][nc] == 0) { grid[nr][nc] = 2; q.push({nr, nc}); }
                    }
                }
            }
            dist++;
        }
        return -1;
    }
};
