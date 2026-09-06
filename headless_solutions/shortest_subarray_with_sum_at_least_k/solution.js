/**
 * @param {number[]} nums
 * @param {number} k
 * @return {number}
 */
var shortestSubarray = function(nums, k) {
    const n = nums.length;
    const P = new BigInt64Array(n + 1);
    for (let i = 0; i < n; i++) P[i + 1] = P[i] + BigInt(nums[i]);
    
    let res = n + 1;
    let dq = [];
    let head = 0;
    
    for (let i = 0; i <= n; i++) {
        while (head < dq.length && P[i] - P[dq[head]] >= BigInt(k)) {
            res = Math.min(res, i - dq[head++]);
        }
        while (head < dq.length && P[i] <= P[dq[dq.length - 1]]) {
            dq.pop();
        }
        dq.push(i);
    }
    return res <= n ? res : -1;
};
