var updateBoard = function(board, click) {
    const [r, c] = click;
    if (board[r][c] === 'M') {
        board[r][c] = 'X';
        return board;
    }

    const dfs = (r, c) => {
        let mines = 0;
        for (let i = -1; i <= 1; i++) {
            for (let j = -1; j <= 1; j++) {
                if (i === 0 && j === 0) continue;
                let nr = r + i, nc = c + j;
                if (nr >= 0 && nr < board.length && nc >= 0 && nc < board[0].length && board[nr][nc] === 'M') mines++;
            }
        }

        if (mines > 0) {
            board[r][c] = mines.toString();
        } else {
            board[r][c] = 'B';
            for (let i = -1; i <= 1; i++) {
                for (let j = -1; j <= 1; j++) {
                    if (i === 0 && j === 0) continue;
                    let nr = r + i, nc = c + j;
                    if (nr >= 0 && nr < board.length && nc >= 0 && nc < board[0].length && board[nr][nc] === 'E') dfs(nr, nc);
                }
            }
        }
    };

    dfs(r, c);
    return board;
};
