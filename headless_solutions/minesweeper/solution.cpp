class Solution {
public:
    vector<vector<char>> updateBoard(vector<vector<char>>& board, vector<int>& click) {
        int r = click[0], c = click[1];
        if (board[r][c] == 'M') {
            board[r][c] = 'X';
            return board;
        }
        dfs(board, r, c);
        return board;
    }

private:
    void dfs(vector<vector<char>>& board, int r, int c) {
        int m = board.size(), n = board[0].size();
        int mines = 0;
        for (int i = -1; i <= 1; ++i) {
            for (int j = -1; j <= 1; ++j) {
                if (i == 0 && j == 0) continue;
                int nr = r + i, nc = c + j;
                if (nr >= 0 && nr < m && nc >= 0 && nc < n && board[nr][nc] == 'M') mines++;
            }
        }

        if (mines > 0) {
            board[r][c] = mines + '0';
        } else {
            board[r][c] = 'B';
            for (int i = -1; i <= 1; ++i) {
                for (int j = -1; j <= 1; ++j) {
                    if (i == 0 && j == 0) continue;
                    int nr = r + i, nc = c + j;
                    if (nr >= 0 && nr < m && nc >= 0 && nc < n && board[nr][nc] == 'E') dfs(board, nr, nc);
                }
            }
        }
    }
};
