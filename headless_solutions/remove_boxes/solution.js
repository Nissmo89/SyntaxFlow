var removeBoxes = function(boxes) {
    const n = boxes.length;
    const memo = Array.from({ length: n }, () => 
        Array.from({ length: n }, () => new Int32Array(n).fill(0))
    );
    
    const solve = (i, j, k) => {
        if (i > j) return 0;
        if (memo[i][j][k] !== 0) return memo[i][j][k];
        
        let i0 = i, k0 = k;
        while (i + 1 <= j && boxes[i + 1] === boxes[i]) {
            i++; k++;
        }
        
        let res = (k + 1) * (k + 1) + solve(i + 1, j, 0);
        for (let m = i + 1; m <= j; m++) {
            if (boxes[m] === boxes[i]) {
                res = Math.max(res, solve(i + 1, m - 1, 0) + solve(m, j, k + 1));
            }
        }
        return memo[i0][j][k0] = res;
    };
    return solve(0, n - 1, 0);
};
