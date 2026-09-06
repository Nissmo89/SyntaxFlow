/**
 * @param {string} expression
 * @return {boolean}
 */
var parseBoolExpr = function(expression) {
    let stack = [];
    for (let char of expression) {
        if (char === ',' || char === '(') continue;
        if (char === ')') {
            let seen = [];
            while (stack.length > 0 && stack[stack.length - 1] !== '!' && 
                   stack[stack.length - 1] !== '&' && stack[stack.length - 1] !== '|') {
                seen.push(stack.pop());
            }
            let op = stack.pop();
            if (op === '!') stack.push(seen[0] === 'f' ? 't' : 'f');
            else if (op === '&') stack.push(seen.includes('f') ? 'f' : 't');
            else stack.push(seen.includes('t') ? 't' : 'f');
        } else {
            stack.push(char);
        }
    }
    return stack[0] === 't';
};
