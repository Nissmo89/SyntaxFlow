var soupServings = function(n) {
    if (n > 5000) return 1.0;
    const memo = Array.from({ length: 200 }, () => Array(200).fill(0));
    const dp = (a, b) => {
        if (a <= 0 && b <= 0) return 0.5;
        if (a <= 0) return 1.0;
        if (b <= 0) return 0.0;
        if (memo[a][b] > 0) return memo[a][b];
        return memo[a][b] = 0.25 * (dp(a-4, b) + dp(a-3, b-1) + dp(a-2, b-2) + dp(a-1, b-3));
    };
    return dp(Math.ceil(n / 25), Math.ceil(n / 25));
};
