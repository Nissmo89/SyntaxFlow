/**
 * @param {string} s
 * @return {number}
 */
var calculate = function(s) {
    let stack = [];
    let num = 0;
    let sign = '+';
    for (let i = 0; i < s.length; i++) {
        if (s[i].match(/[0-9]/)) {
            num = num * 10 + parseInt(s[i]);
        }
        if ((!s[i].match(/[0-9]/) && s[i] != ' ') || i == s.length - 1) {
            if (sign == '-') {
                stack.push(-num);
            }
            if (sign == '+') {
                stack.push(num);
            }
            if (sign == '*') {
                stack.push(stack.pop() * num);
            }
            if (sign == '/') {
                stack.push(Math.floor(stack.pop() / num));
            }
            sign = s[i];
            num = 0;
        }
    }

    return stack.reduce((a, b) => a + b, 0);
};
