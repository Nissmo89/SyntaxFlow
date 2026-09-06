/**
 * @param {string} expression
 * @return {number[]}
 */
var diffWaysToCompute = function(expression, memo = new Map()) {
    if (memo.has(expression)) return memo.get(expression);
    let res = [];
    for (let i = 0; i < expression.length; i++) {
        let char = expression[i];
        if (char === '+' || char === '-' || char === '*') {
            let left = diffWaysToCompute(expression.substring(0, i), memo);
            let right = diffWaysToCompute(expression.substring(i + 1), memo);
            for (let l of left) {
                for (let r of right) {
                    if (char === '+') res.push(l + r);
                    else if (char === '-') res.push(l - r);
                    else res.push(l * r);
                }
            }
        }
    }
    if (res.length === 0) res.push(parseInt(expression));
    memo.set(expression, res);
    return res;
};
