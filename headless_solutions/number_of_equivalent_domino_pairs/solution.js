/**
 * @param {number[][]} dominoes
 * @return {number}
 */
var numEquivDominoPairs = function(dominoes) {
    let count = {};
    let res = 0;
    for (let domino of dominoes) {
        let key = min(domino[0], domino[1]) + "" + max(domino[0], domino[1]);
        count[key] = (count[key] || 0) + 1;
    }
    for (let pair in count) {
        res += count[pair] * (count[pair] - 1) / 2;
    }
    return res;
};
