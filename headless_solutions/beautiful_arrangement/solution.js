/**
 * @param {number} n
 * @return {number}
 */
var countArrangement = function(n) {
    let bm = 0, pl = n;
    function dfs(bm, pl) {
        if (pl == 0) return 1;
        let S = 0;
        for (let i = 1; i <= n; i++) {
            if (!(bm & (1 << (i - 1))) && ((i % pl == 0) || (pl % i == 0))) {
                S += dfs(bm | (1 << (i - 1)), pl - 1);
            }
        }
        return S;
    }
    return dfs(bm, pl);
};
