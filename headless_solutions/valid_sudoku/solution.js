/**
 * @param {character[][]} board
 * @return {boolean}
 */
var isValidSudoku = function(board) {
    const rows = Array.from({ length: 9 }, () => new Array(9).fill(false));
    const cols = Array.from({ length: 9 }, () => new Array(9).fill(false));
    const boxes = Array.from({ length: 9 }, () => new Array(9).fill(false));
    
    for (let i = 0; i < 9; i++) {
        for (let j = 0; j < 9; j++) {
            if (board[i][j] !== '.') {
                const num = parseInt(board[i][j]) - 1;
                const k = Math.floor(i / 3) * 3 + Math.floor(j / 3);
                if (rows[i][num] || cols[j][num] || boxes[k][num]) return false;
                rows[i][num] = cols[j][num] = boxes[k][num] = true;
            }
        }
    }
    return true;
};
