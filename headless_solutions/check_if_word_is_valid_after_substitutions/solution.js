/**
 * @param {string} s
 * @return {boolean}
 */
var isValid = function(s) {
    let stack = [];
    for (let c of s) {
        if (c === 'c') {
            if (stack.length === 0 || stack.pop() !== 'b') {
                return false;
            }
            if (stack.length === 0 || stack.pop() !== 'a') {
                return false;
            }
        } else {
            stack.push(c);
        }
    }
    return stack.length === 0;
};
