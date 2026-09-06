class Solution {
public:
    vector<vector<int>> colorBorder(vector<vector<int>>& grid, int row, int col, int color) {
        if (grid.empty()) return grid;
        const int m = grid.size(), n = grid[0].size();
        const int original = grid[row][col];
        // If the target colour equals the original, the grid stays unchanged.
        if (original == color) return grid;

        vector<vector<bool>> visited(m, vector<bool>(n, false));
        vector<pair<int,int>> component;
        const int dirs[4][2] = {{-1,0},{1,0},{0,-1},{0,1}};

        // -------- DFS to collect the component --------
        function<void(int,int)> dfs = [&](int r, int c) {
            if (r < 0 || r >= m || c < 0 || c >= n) return;
            if (visited[r][c] || grid[r][c] != original) return;
            visited[r][c] = true;
            component.emplace_back(r, c);
            for (auto& d : dirs) {
                dfs(r + d[0], c + d[1]);
            }
        };
        dfs(row, col);

        // -------- Identify border cells and recolour --------
        for (auto [r, c] : component) {
            bool isBorder = false;
            for (auto& d : dirs) {
                int nr = r + d[0], nc = c + d[1];
                if (nr < 0 || nr >= m || nc < 0 || nc >= n || grid[nr][nc] != original) {
                    isBorder = true;
                    break;
                }
            }
            if (isBorder) {
                grid[r][c] = color;
            }
        }
        return grid;
    }
};
