/**
 * @param {number[]} nums
 * @return {boolean}
 */
var splitArraySameAverage = function(nums) {
    const n = nums.length, S = nums.reduce((a, b) => a + b, 0);
    const m = Math.floor(n / 2);
    const left = Array.from({ length: m + 1 }, () => new Set([0]));
    for (const x of nums.slice(0, m)) {
        for (let i = m - 1; i >= 0; i--) {
            for (const s of left[i]) left[i + 1].add(s + x);
        }
    }
    const right = Array.from({ length: n - m + 1 }, () => new Set([0]));
    for (const x of nums.slice(m)) {
        for (let i = n - m - 1; i >= 0; i--) {
            for (const s of right[i]) right[i + 1].add(s + x);
        }
    }
    for (let k = 1; k <= Math.floor(n / 2); k++) {
        if ((S * k) % n === 0) {
            const target = (S * k) / n;
            for (let i = Math.max(0, k - (n - m)); i <= Math.min(k, m); i++) {
                for (const s of left[i]) if (right[k - i].has(target - s)) return true;
            }
        }
    }
    return false;
};
