class Solution {
public:
    int matrixScore(vector<vector<int>>& grid) {
        int M = grid.size(), N = grid[0].size();
        // Every row starts with 1, contributing 2^(N-1) each
        int res = (1 << (N - 1)) * M;
        
        for (int j = 1; j < N; ++j) {
            int count = 0;
            for (int i = 0; i < M; ++i) {
                // If grid[i][0] == 1, bit is grid[i][j]. 
                // If grid[i][0] == 0, row was flipped, bit is 1 - grid[i][j].
                if (grid[i][j] == grid[i][0]) count++;
            }
            res += max(count, M - count) * (1 << (N - 1 - j));
        }
        return res;
    }
};
