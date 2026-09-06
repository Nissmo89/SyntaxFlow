var totalNQueens = function(n) {
    let count = 0;
    const cols = new Array(n).fill(false);
    const d1 = new Array(2 * n).fill(false);
    const d2 = new Array(2 * n).fill(false);
    
    const backtrack = (row) => {
        if (row === n) { count++; return; }
        for (let col = 0; col < n; col++) {
            if (!cols[col] && !d1[row - col + n] && !d2[row + col]) {
                cols[col] = d1[row - col + n] = d2[row + col] = true;
                backtrack(row + 1);
                cols[col] = d1[row - col + n] = d2[row + col] = false;
            }
        }
    };
    backtrack(0);
    return count;
};
