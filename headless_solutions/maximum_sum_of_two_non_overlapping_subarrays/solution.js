var maxSumTwoNoOverlap = function(nums, firstLen, secondLen) {
    const n = nums.length;
    const ps = new Array(n + 1).fill(0);
    for (let i = 0; i < n; i++) ps[i + 1] = ps[i] + nums[i];
    
    const solve = (L, M) => {
        let res = 0, maxL = 0;
        for (let i = L + M; i <= n; i++) {
            maxL = Math.max(maxL, ps[i - M] - ps[i - M - L]);
            res = Math.max(res, maxL + ps[i] - ps[i - M]);
        }
        return res;
    };
    return Math.max(solve(firstLen, secondLen), solve(secondLen, firstLen));
};
