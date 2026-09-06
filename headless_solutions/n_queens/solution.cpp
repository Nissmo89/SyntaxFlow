class Solution {
public:
    vector<vector<string>> solveNQueens(int n) {
        vector<vector<string>> res;
        vector<string> board(n, string(n, '.'));
        vector<bool> cols(n, false), d1(2 * n, false), d2(2 * n, false);
        backtrack(0, n, board, res, cols, d1, d2);
        return res;
    }
    void backtrack(int r, int n, vector<string>& board, vector<vector<string>>& res, 
                   vector<bool>& cols, vector<bool>& d1, vector<bool>& d2) {
        if (r == n) { res.push_back(board); return; }
        for (int c = 0; c < n; ++c) {
            if (!cols[c] && !d1[r - c + n] && !d2[r + c]) {
                board[r][c] = 'Q';
                cols[c] = d1[r - c + n] = d2[r + c] = true;
                backtrack(r + 1, n, board, res, cols, d1, d2);
                board[r][c] = '.';
                cols[c] = d1[r - c + n] = d2[r + c] = false;
            }
        }
    }
};
