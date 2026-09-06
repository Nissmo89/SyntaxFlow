/**
 * @param {string} expression
 * @return {string[]}
 */
var braceExpansionII = function(expression) {
    let stack = [], res = [], cur = new Set(['']);
    for (let char of expression) {
        if (/[a-z]/.test(char)) {
            let next = new Set();
            for (let s of cur) next.add(s + char);
            cur = next;
        } else if (char === '{') {
            stack.push([res, cur]);
            res = []; cur = new Set(['']);
        } else if (char === '}') {
            res.push(cur);
            let combined = new Set();
            for (let group of res) for (let s of group) combined.add(s);
            let [prevRes, prevCur] = stack.pop();
            cur = new Set();
            for (let p of prevCur) for (let c of combined) cur.add(p + c);
            res = prevRes;
        } else if (char === ',') {
            res.push(cur);
            cur = new Set(['']);
        }
    }
    res.push(cur);
    let finalSet = new Set();
    for (let group of res) for (let s of group) finalSet.add(s);
    return Array.from(finalSet).sort();
};
