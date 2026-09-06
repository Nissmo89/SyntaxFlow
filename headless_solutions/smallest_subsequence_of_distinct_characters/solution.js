var smallestSubsequence = function(s) {
    const last = {};
    for (let i = 0; i < s.length; i++) last[s[i]] = i;
    const stack = [];
    const seen = new Set();
    for (let i = 0; i < s.length; i++) {
        const char = s[i];
        if (seen.has(char)) continue;
        while (stack.length > 0 && stack[stack.length - 1] > char && i < last[stack[stack.length - 1]]) {
            seen.delete(stack.pop());
        }
        stack.push(char);
        seen.add(char);
    }
    return stack.join('');
};
