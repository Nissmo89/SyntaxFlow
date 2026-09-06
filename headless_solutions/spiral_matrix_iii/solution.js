var spiralMatrixIII = function(rows, cols, rStart, cStart) {
    let res = [];
    let r = rStart, c = cStart;
    let dr = [0, 1, 0, -1], dc = [1, 0, -1, 0];
    let step = 1, di = 0;
    
    while (res.length < rows * cols) {
        for (let i = 0; i < 2; i++) {
            for (let j = 0; j < step; j++) {
                if (r >= 0 && r < rows && c >= 0 && c < cols)
                    res.push([r, c]);
                r += dr[di];
                c += dc[di];
            }
            di = (di + 1) % 4;
        }
        step++;
    }
    return res;
};
