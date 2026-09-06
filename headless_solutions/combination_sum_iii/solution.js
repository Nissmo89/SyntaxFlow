/**
 * @param {number} k
 * @param {number} n
 * @return {number[][]}
 */
var combinationSum3 = function(k, n) {
    let ans = [];
    combination(ans, [], k, 1, n);
    return ans;
};

function combination(ans, comb, k, start, n) {
    if (comb.length == k && n == 0) {
        ans.push([...comb]);
        return;
    }
    for (let i = start; i <= 9; i++) {
        comb.push(i);
        combination(ans, comb, k, i+1, n-i);
        comb.pop();
    }
}
