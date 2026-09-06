class Solution {
public:
    int totalNQueens(int n) {
        vector<bool> cols(n, false), d1(2 * n, false), d2(2 * n, false);
        int count = 0;
        backtrack(0, n, cols, d1, d2, count);
        return count;
    }

    void backtrack(int row, int n, vector<bool>& cols, vector<bool>& d1, vector<bool>& d2, int& count) {
        if (row == n) { count++; return; }
        for (int col = 0; col < n; ++col) {
            int id1 = row - col + n, id2 = row + col;
            if (cols[col] || d1[id1] || d2[id2]) continue;
            cols[col] = d1[id1] = d2[id2] = true;
            backtrack(row + 1, n, cols, d1, d2, count);
            cols[col] = d1[id1] = d2[id2] = false;
        }
    }
};
