var numMagicSquaresInside = function(grid) {
    const isMagic = (r, c) => {
        const nums = [];
        for (let i = 0; i < 3; i++) for (let j = 0; j < 3; j++) nums.push(grid[r + i][c + j]);
        const set = new Set(nums);
        if (set.size !== 9 || Math.min(...nums) !== 1 || Math.max(...nums) !== 9) return false;
        const sums = [
            grid[r][c] + grid[r][c+1] + grid[r][c+2],
            grid[r+1][c] + grid[r+1][c+1] + grid[r+1][c+2],
            grid[r+2][c] + grid[r+2][c+1] + grid[r+2][c+2],
            grid[r][c] + grid[r+1][c] + grid[r+2][c],
            grid[r][c+1] + grid[r+1][c+1] + grid[r+2][c+1],
            grid[r][c+2] + grid[r+1][c+2] + grid[r+2][c+2],
            grid[r][c] + grid[r+1][c+1] + grid[r+2][c+2],
            grid[r][c+2] + grid[r+1][c+1] + grid[r+2][c]
        ];
        return sums.every(s => s === 15);
    };
    let count = 0;
    for (let i = 0; i <= grid.length - 3; i++) {
        for (let j = 0; j <= grid[0].length - 3; j++) {
            if (isMagic(i, j)) count++;
        }
    }
    return count;
};
