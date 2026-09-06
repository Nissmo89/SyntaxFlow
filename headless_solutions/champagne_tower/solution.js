/**
 * @param {number} poured
 * @param {number} query_row
 * @param {number} query_glass
 * @return {number}
 */
var champagneTower = function(poured, query_row, query_glass) {
    const dp = Array.from({ length: query_row + 2 }, () => Array(query_row + 2).fill(0));
    dp[0][0] = poured;

    for (let r = 0; r <= query_row; r++) {
        for (let c = 0; c <= r; c++) {
            if (dp[r][c] > 1) {
                const overflow = (dp[r][c] - 1) / 2;
                dp[r + 1][c]     += overflow;
                dp[r + 1][c + 1] += overflow;
                dp[r][c] = 1;
            }
        }
    }
    return dp[query_row][query_glass];
};
