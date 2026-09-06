/**
 * @param {string} equation
 * @return {string}
 */
var solveEquation = function(equation) {
    const parse = (exp) => {
        let x = 0, n = 0;
        const tokens = exp.match(/([+-]?\d*x)|([+-]?\d+)/g) || [];
        for (let t of tokens) {
            if (t.includes('x')) {
                let val = t.replace('x', '');
                x += (val === '' || val === '+') ? 1 : (val === '-' ? -1 : parseInt(val));
            } else {
                n += parseInt(t);
            }
        }
        return [x, n];
    };
    const [left, right] = equation.split('=');
    const [lx, ln] = parse(left), [rx, rn] = parse(right);
    const x = lx - rx, n = rn - ln;
    if (x === 0) return n === 0 ? "Infinite solutions" : "No solution";
    return `x=${n / x}`;
};
