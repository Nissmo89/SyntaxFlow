var movesToChessboard = function(board) {
    const n = board.length;
    for (let i = 0; i < n; i++)
        for (let j = 0; j < n; j++)
            if ((board[0][0] ^ board[i][0] ^ board[0][j] ^ board[i][j]) === 1) return -1;
    
    let rowSum = 0, colSum = 0, rowSwap = 0, colSwap = 0;
    for (let i = 0; i < n; i++) {
        rowSum += board[0][i];
        colSum += board[i][0];
        if (board[i][0] === i % 2) rowSwap++;
        if (board[0][i] === i % 2) colSwap++;
    }
    if (rowSum < Math.floor(n / 2) || rowSum > Math.ceil(n / 2) || 
        colSum < Math.floor(n / 2) || colSum > Math.ceil(n / 2)) return -1;
    
    if (n % 2 === 1) {
        if (rowSwap % 2 !== 0) rowSwap = n - rowSwap;
        if (colSwap % 2 !== 0) colSwap = n - colSwap;
    } else {
        rowSwap = Math.min(rowSwap, n - rowSwap);
        colSwap = Math.min(colSwap, n - colSwap);
    }
    return (rowSwap + colSwap) / 2;
};
