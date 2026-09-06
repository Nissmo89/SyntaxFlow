class Solution {
public:
    vector<vector<int>> spiralMatrixIII(int rows, int cols, int rStart, int cStart) {
        vector<vector<int>> res;
        int r = rStart, c = cStart;
        int dr[] = {0, 1, 0, -1}, dc[] = {1, 0, -1, 0};
        int step = 1, di = 0;
        
        while (res.size() < rows * cols) {
            for (int i = 0; i < 2; ++i) { // Two segments per step increment
                for (int j = 0; j < step; ++j) {
                    if (r >= 0 && r < rows && c >= 0 && c < cols)
                        res.push_back({r, c});
                    r += dr[di];
                    c += dc[di];
                }
                di = (di + 1) % 4;
            }
            step++;
        }
        return res;
    }
};
