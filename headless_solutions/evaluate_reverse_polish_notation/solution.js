/**
 * @param {string[]} tokens
 * @return {number}
 */
var evalRPN = function(tokens) {
    const stack = [];
    for (const t of tokens) {
        if (t === "+" || t === "-" || t === "*" || t === "/") {
            const b = stack.pop();
            const a = stack.pop();
            if (t === "+") stack.push(a + b);
            else if (t === "-") stack.push(a - b);
            else if (t === "*") stack.push(a * b);
            else stack.push(Math.trunc(a / b)); // Truncate toward zero
        } else {
            stack.push(parseInt(t, 10));
        }
    }
    return stack.pop();
};
