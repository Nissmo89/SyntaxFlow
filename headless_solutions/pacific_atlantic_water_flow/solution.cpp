class Solution {
    int m, n;
    int dirs[4][2] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};
    void dfs(int r, int c, vector<vector<bool>>& visited, vector<vector<int>>& heights) {
        visited[r][c] = true;
        for (auto& d : dirs) {
            int nr = r + d[0], nc = c + d[1];
            if (nr >= 0 && nr < m && nc >= 0 && nc < n && !visited[nr][nc] && heights[nr][nc] >= heights[r][c])
                dfs(nr, nc, visited, heights);
        }
    }
public:
    vector<vector<int>> pacificAtlantic(vector<vector<int>>& heights) {
        if (heights.empty()) return {};
        m = heights.size(); n = heights[0].size();
        vector<vector<bool>> pac(m, vector<bool>(n, false)), atl(m, vector<bool>(n, false));
        for (int i = 0; i < m; ++i) { dfs(i, 0, pac, heights); dfs(i, n - 1, atl, heights); }
        for (int j = 0; j < n; ++j) { dfs(0, j, pac, heights); dfs(m - 1, j, atl, heights); }
        vector<vector<int>> res;
        for (int i = 0; i < m; ++i)
            for (int j = 0; j < n; ++j)
                if (pac[i][j] && atl[i][j]) res.push_back({i, j});
        return res;
    }
};
