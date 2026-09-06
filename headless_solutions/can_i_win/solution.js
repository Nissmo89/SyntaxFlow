// JavaScript implementation matching signature
/**
 * @param {number} maxChoosableInteger
 * @param {number} desiredTotal
 * @return {boolean}
 */
var canIWin = function(maxChoosableInteger, desiredTotal) {
    if (desiredTotal <= 0) return true;
    const maxSum = (maxChoosableInteger + 1) * maxChoosableInteger / 2;
    if (maxSum < desiredTotal) return false;
    if (maxChoosableInteger >= desiredTotal) return true;

    const memo = new Map(); // key: mask, value: true/false

    const dfs = (mask, rem) => {
        if (memo.has(mask)) return memo.get(mask);
        for (let i = 0; i < maxChoosableInteger; i++) {
            if ((mask & (1 << i)) === 0) {
                const val = i + 1;
                if (val >= rem) {
                    memo.set(mask, true);
                    return true;
                }
                if (!dfs(mask | (1 << i), rem - val)) {
                    memo.set(mask, true);
                    return true;
                }
            }
        }
        memo.set(mask, false);
        return false;
    };

    return dfs(0, desiredTotal);
};
