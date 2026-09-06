class Solution {
public:
    vector<vector<int>> generateMatrix(int n) {
        vector<vector<int>> matrix(n, vector<int>(n, 0));
        int r = 0, c = 0, dr = 0, dc = 1;
        for (int i = 1; i <= n * n; ++i) {
            matrix[r][c] = i;
            int nr = r + dr, nc = c + dc;
            if (nr < 0 || nr >= n || nc < 0 || nc >= n || matrix[nr][nc] != 0) {
                int temp = dr;
                dr = dc;
                dc = -temp;
            }
            r += dr;
            c += dc;
        }
        return matrix;
    }
};
