class Solution {
public:
    int numMagicSquaresInside(vector<vector<int>>& grid) {
        int rows = grid.size(), cols = grid[0].size(), count = 0;
        for (int i = 0; i <= rows - 3; ++i) {
            for (int j = 0; j <= cols - 3; ++j) {
                if (isMagic(grid, i, j)) count++;
            }
        }
        return count;
    }
private:
    bool isMagic(vector<vector<int>>& g, int r, int c) {
        int seen[10] = {0};
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                int val = g[r + i][c + j];
                if (val < 1 || val > 9 || seen[val]) return false;
                seen[val] = 1;
            }
        }
        int s1 = g[r][c] + g[r][c+1] + g[r][c+2];
        int s2 = g[r+1][c] + g[r+1][c+1] + g[r+1][c+2];
        int s3 = g[r+2][c] + g[r+2][c+1] + g[r+2][c+2];
        int c1 = g[r][c] + g[r+1][c] + g[r+2][c];
        int c2 = g[r][c+1] + g[r+1][c+1] + g[r+2][c+1];
        int c3 = g[r][c+2] + g[r+1][c+2] + g[r+2][c+2];
        int d1 = g[r][c] + g[r+1][c+1] + g[r+2][c+2];
        int d2 = g[r][c+2] + g[r+1][c+1] + g[r+2][c];
        return (s1 == 15 && s2 == 15 && s3 == 15 && c1 == 15 && c2 == 15 && c3 == 15 && d1 == 15 && d2 == 15);
    }
};
