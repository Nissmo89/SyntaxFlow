var lenLongestFibSubseq = function(arr) {
    const n = arr.length;
    const map = new Map();
    for (let i = 0; i < n; i++) map.set(arr[i], i);
    
    const dp = Array.from({ length: n }, () => new Array(n).fill(0));
    let maxLen = 0;
    
    for (let i = 0; i < n; i++) {
        for (let j = 0; j < i; j++) {
            const diff = arr[i] - arr[j];
            if (diff < arr[j] && map.has(diff)) {
                const k = map.get(diff);
                dp[j][i] = dp[k][j] + 1;
                maxLen = Math.max(maxLen, dp[j][i] + 2);
            }
        }
    }
    return maxLen >= 3 ? maxLen : 0;
};
