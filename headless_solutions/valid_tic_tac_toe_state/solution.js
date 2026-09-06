var validTicTacToe = function(board) {
    let x = 0, o = 0;
    for (let row of board) {
        for (let char of row) {
            if (char === 'X') x++;
            else if (char === 'O') o++;
        }
    }
    const win = (p) => {
        for (let i = 0; i < 3; i++) {
            if (board[i][0] === p && board[i][1] === p && board[i][2] === p) return true;
            if (board[0][i] === p && board[1][i] === p && board[2][i] === p) return true;
        }
        return (board[0][0] === p && board[1][1] === p && board[2][2] === p) ||
               (board[0][2] === p && board[1][1] === p && board[2][0] === p);
    };
    if (o > x || x > o + 1) return false;
    if (win('X') && x !== o + 1) return false;
    if (win('O') && x !== o) return false;
    if (win('X') && win('O')) return false;
    return true;
};
