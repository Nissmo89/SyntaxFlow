/**
 * @param {string} s
 * @return {number[][]}
 */
var largeGroupPositions = function(s) {
    let res = [];
    for (let i = 0, j = 0; i < s.length; i = j) {
        while (j < s.length && s[j] == s[i]) ++j;
        if (j - i >= 3)
            res.push([i, j - 1]);
    }
    return res;
};
