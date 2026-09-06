class Solution {
public:
    int swimInWater(vector<vector<int>>& grid) {
        int n = grid.size();
        priority_queue<vector<int>, vector<vector<int>>, greater<vector<int>>> pq;
        vector<vector<bool>> visited(n, vector<bool>(n, false));
        
        pq.push({grid[0][0], 0, 0});
        visited[0][0] = true;
        int res = 0;
        int dirs[4][2] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};
        
        while (!pq.empty()) {
            auto curr = pq.top(); pq.pop();
            res = max(res, curr[0]);
            if (curr[1] == n - 1 && curr[2] == n - 1) return res;
            
            for (auto& d : dirs) {
                int ni = curr[1] + d[0], nj = curr[2] + d[1];
                if (ni >= 0 && ni < n && nj >= 0 && nj < n && !visited[ni][nj]) {
                    visited[ni][nj] = true;
                    pq.push({grid[ni][nj], ni, nj});
                }
            }
        }
        return res;
    }
};
