class Solution {
public:
    vector<vector<int>> updateMatrix(vector<vector<int>>& mat) {
        int m = mat.size();
        int n = mat[0].size();
        
        vector<vector<int>> dist(m, vector<int>(n, INT_MAX));
        
        queue<pair<int, int>> q;
        
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (mat[i][j] == 0) {
                    dist[i][j] = 0;
                    q.push({i, j});
                }
            }
        }
        
        vector<pair<int, int>> dirs = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
        
        while (!q.empty()) {
            auto cell = q.front();
            q.pop();
            
            for (auto dir : dirs) {
                int r = cell.first + dir.first;
                int c = cell.second + dir.second;
                
                if (r < 0 || r >= m || c < 0 || c >= n || dist[r][c] <= dist[cell.first][cell.second] + 1) continue;
                
                dist[r][c] = dist[cell.first][cell.second] + 1;
                q.push({r, c});
            }
        }
        
        return dist;
    }
};
