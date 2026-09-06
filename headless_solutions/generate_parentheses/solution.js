/**
 * @param {number} n
 * @return {string[]}
 */
var generateParenthesis = function(n) {
    const res = [];
    const backtrack = (s, open, close) => {
        if (s.length === 2 * n) {
            res.push(s);
            return;
        }
        if (open < n) backtrack(s + "(", open + 1, close);
        if (close < open) backtrack(s + ")", open, close + 1);
    };
    backtrack("", 0, 0);
    return res;
};
