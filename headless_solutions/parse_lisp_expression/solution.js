var evaluate = function(expression) {
    const parse = (s) => {
        let res = [], bal = 0, cur = "";
        for (let char of s) {
            if (char === '(') bal++; else if (char === ')') bal--;
            if (char === ' ' && bal === 0) { res.push(cur); cur = ""; }
            else cur += char;
        }
        res.push(cur);
        return res;
    };

    const solve = (expr, scope) => {
        if (expr[0] !== '(') {
            if (!isNaN(expr) || expr[0] === '-') return parseInt(expr);
            return scope[expr];
        }
        let tokens = parse(expr.slice(1, -1));
        let newScope = Object.assign({}, scope);
        if (tokens[0] === "add") return solve(tokens[1], newScope) + solve(tokens[2], newScope);
        if (tokens[0] === "mult") return solve(tokens[1], newScope) * solve(tokens[2], newScope);
        for (let i = 1; i < tokens.length - 1; i += 2) newScope[tokens[i]] = solve(tokens[i + 1], newScope);
        return solve(tokens[tokens.length - 1], newScope);
    };
    return solve(expression, {});
};
