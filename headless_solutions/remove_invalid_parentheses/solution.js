var removeInvalidParentheses = function(s) {
    const isValid = (str) => {
        let count = 0;
        for (let char of str) {
            if (char === '(') count++;
            else if (char === ')') {
                count--;
                if (count < 0) return false;
            }
        }
        return count === 0;
    };
    let res = [], queue = [s], visited = new Set([s]), found = false;
    while (queue.length > 0) {
        let curr = queue.shift();
        if (isValid(curr)) {
            res.push(curr);
            found = true;
        }
        if (found) continue;
        for (let i = 0; i < curr.length; i++) {
            if (curr[i] !== '(' && curr[i] !== ')') continue;
            let next = curr.slice(0, i) + curr.slice(i + 1);
            if (!visited.has(next)) {
                visited.add(next);
                queue.push(next);
            }
        }
    }
    return res;
};
