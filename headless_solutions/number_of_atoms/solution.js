var countOfAtoms = function(formula) {
    let stack = [], curr = {}, i = 0;
    while (i < formula.length) {
        if (formula[i] === '(') {
            stack.push(curr);
            curr = {};
            i++;
        } else if (formula[i] === ')') {
            i++;
            let start = i;
            while (i < formula.length && /\d/.test(formula[i])) i++;
            let mult = parseInt(formula.substring(start, i) || 1);
            let prev = stack.pop();
            for (let atom in curr) {
                prev[atom] = (prev[atom] || 0) + curr[atom] * mult;
            }
            curr = prev;
        } else {
            let start = i++;
            while (i < formula.length && /[a-z]/.test(formula[i])) i++;
            let name = formula.substring(start, i);
            start = i;
            while (i < formula.length && /\d/.test(formula[i])) i++;
            let count = parseInt(formula.substring(start, i) || 1);
            curr[name] = (curr[name] || 0) + count;
        }
    }
    return Object.keys(curr).sort().map(k => k + (curr[k] > 1 ? curr[k] : '')).join('');
};
