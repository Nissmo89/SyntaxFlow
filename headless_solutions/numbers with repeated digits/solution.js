/**
 * @param {number} n
 * @return {number}
 */
var numDupDigitsAtMostN = function(n) {
    const s = n.toString();
    const len = s.length;
    const P = (n, k) => {
        let res = 1;
        for (let i = 0; i < k; i++) res *= (n - i);
        return res;
    };

    let count = 0;
    for (let i = 1; i < len; i++) count += 9 * P(9, i - 1);

    let used = new Array(10).fill(false);
    for (let i = 0; i < len; i++) {
        let digit = parseInt(s[i]);
        for (let j = (i === 0 ? 1 : 0); j < digit; j++) {
            if (!used[j]) count += P(9 - i, len - 1 - i);
        }
        if (used[digit]) return n - count;
        used[digit] = true;
    }
    return n - (count + 1);
};
