var numSubmatrixSumTarget = function(matrix, target) {
    const m = matrix.length, n = matrix[0].length;
    for (let i = 0; i < m; i++)
        for (let j = 1; j < n; j++)
            matrix[i][j] += matrix[i][j - 1];
    
    let res = 0;
    for (let i = 0; i < n; i++) {
        for (let j = i; j < n; j++) {
            let map = new Map([[0, 1]]);
            let cur = 0;
            for (let k = 0; k < m; k++) {
                cur += matrix[k][j] - (i > 0 ? matrix[k][i - 1] : 0);
                res += map.get(cur - target) || 0;
                map.set(cur, (map.get(cur) || 0) + 1);
            }
        }
    }
    return res;
};
