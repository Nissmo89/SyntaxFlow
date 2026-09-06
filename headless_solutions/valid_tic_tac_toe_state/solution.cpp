class Solution {
public:
    bool win(vector<string>& b, char p) {
        for (int i = 0; i < 3; i++) {
            if (b[i][0] == p && b[i][1] == p && b[i][2] == p) return true;
            if (b[0][i] == p && b[1][i] == p && b[2][i] == p) return true;
        }
        return (b[0][0] == p && b[1][1] == p && b[2][2] == p) ||
               (b[0][2] == p && b[1][1] == p && b[2][0] == p);
    }

    bool validTicTacToe(vector<string>& board) {
        int x = 0, o = 0;
        for (const string& row : board)
            for (char c : row) {
                if (c == 'X') x++;
                if (c == 'O') o++;
            }
        if (o > x || x > o + 1) return false;
        if (win(board, 'X') && x != o + 1) return false;
        if (win(board, 'O') && x != o) return false;
        if (win(board, 'X') && win(board, 'O')) return false;
        return true;
    }
};
