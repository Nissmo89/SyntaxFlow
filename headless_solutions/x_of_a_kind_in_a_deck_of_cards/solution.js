/**
 * @param {number[]} deck
 * @return {boolean}
 */
var hasGroupsSizeX = function(deck) {
    let count = {};
    for (let i of deck) count[i] = (count[i] || 0) + 1;
    let res = 0;
    for (let i in count) res = gcd(count[i], res);
    return res > 1;
};
function gcd(a, b) {
    return b > 0 ? gcd(b, a % b) : a;
}
