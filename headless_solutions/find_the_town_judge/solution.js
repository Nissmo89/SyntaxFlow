/**
 * @param {number} n
 * @param {number[][]} trust
 * @return {number}
 */
var findJudge = function (n, trust) {
    let count = new Array(n + 1).fill(0);
    for (let i = 0; i < trust.length; i++) {
        count[trust[i][0]]--;
        count[trust[i][1]]++;
    }
    for (let i = 1; i <= n; i++) {
        if (count[i] == n - 1)
            return i;
    }
    return -1;
};
