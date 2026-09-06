var makesquare = function(matchsticks) {
    const sum = matchsticks.reduce((a, b) => a + b, 0);
    if (sum % 4 !== 0) return false;
    const target = sum / 4;
    matchsticks.sort((a, b) => b - a);
    const sides = new Array(4).fill(0);

    const dfs = (idx) => {
        if (idx === matchsticks.length) return true;
        for (let i = 0; i < 4; i++) {
            if (sides[i] + matchsticks[idx] <= target) {
                sides[i] += matchsticks[idx];
                if (dfs(idx + 1)) return true;
                sides[i] -= matchsticks[idx];
            }
            if (sides[i] === 0) break;
        }
        return false;
    };
    return dfs(0);
};
