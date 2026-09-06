var shortestCommonSupersequence = function(str1, str2) {
    const m = str1.length, n = str2.length;
    const dp = Array.from({ length: m + 1 }, () => new Int32Array(n + 1));
    for (let i = 0; i <= m; i++) dp[i][0] = i;
    for (let j = 0; j <= n; j++) dp[0][j] = j;
    for (let i = 1; i <= m; i++) {
        for (let j = 1; j <= n; j++) {
            if (str1[i - 1] === str2[j - 1]) dp[i][j] = 1 + dp[i - 1][j - 1];
            else dp[i][j] = 1 + Math.min(dp[i - 1][j], dp[i][j - 1]);
        }
    }
    let res = [], i = m, j = n;
    while (i > 0 && j > 0) {
        if (str1[i - 1] === str2[j - 1]) { res.push(str1[i - 1]); i--; j--; }
        else if (dp[i - 1][j] < dp[i][j - 1]) { res.push(str1[i - 1]); i--; }
        else { res.push(str2[j - 1]); j--; }
    }
    while (i > 0) res.push(str1[--i]);
    while (j > 0) res.push(str2[--j]);
    return res.reverse().join('');
};
