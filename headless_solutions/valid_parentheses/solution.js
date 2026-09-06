/**
 * @param {string} s
 * @return {boolean}
 */
var isValid = function(s) {
    let stack = [];
    for (let c of s) {
        if (c === '(' || c === '{' || c === '[') {
            stack.push(c);
        } else if (c === ')' || c === '}' || c === ']') {
            if (!stack.length) {
                return false;
            }
            let top = stack.pop();
            if ((c === ')' && top !== '(') || (c === '}' && top !== '{') || (c === ']' && top !== '[')) {
                return false;
            }
        }
    }
    return !stack.length;
};
