var solveNQueens = function(n) {
    const res = [], board = Array.from({ length: n }, () => Array(n).fill('.'));
    const cols = new Set(), d1 = new Set(), d2 = new Set();
    const backtrack = (r) => {
        if (r === n) {
            res.push(board.map(row => row.join('')));
            return;
        }
        for (let c = 0; c < n; c++) {
            if (!cols.has(c) && !d1.has(r - c) && !d2.has(r + c)) {
                board[r][c] = 'Q';
                cols.add(c); d1.add(r - c); d2.add(r + c);
                backtrack(r + 1);
                board[r][c] = '.';
                cols.delete(c); d1.delete(r - c); d2.delete(r + c);
            }
        }
    };
    backtrack(0);
    return res;
};
