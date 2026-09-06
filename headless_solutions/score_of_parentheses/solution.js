/**
 * @param {string} s
 * @return {number}
 */
var scoreOfParentheses = function(s) {
    let stack = [];
    let cur = 0;
    for (let char of s) {
        if (char === '(') {
            stack.push(cur);
            cur = 0;
        } else {
            cur = stack.pop() + Math.max(2 * cur, 1);
        }
    }
    return cur;
};
