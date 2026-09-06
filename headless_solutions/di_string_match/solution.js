/**
 * @param {string} s
 * @return {number[]}
 */
var diStringMatch = function(s) {
    let res = [];
    let low = 0, high = s.length;
    for (let c of s) {
        if (c === 'I') {
            res.push(low++);
        } else {
            res.push(high--);
        }
    }
    res.push(low); // Append the last value
    return res;
};
