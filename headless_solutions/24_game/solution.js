var judgePoint24 = function(cards) {
    const solve = (nums) => {
        if (nums.length === 1) return Math.abs(nums[0] - 24) < 1e-6;
        for (let i = 0; i < nums.length; i++) {
            for (let j = 0; j < nums.length; j++) {
                if (i === j) continue;
                let next = nums.filter((_, idx) => idx !== i && idx !== j);
                let a = nums[i], b = nums[j];
                let ops = [a + b, a - b, a * b];
                if (Math.abs(b) > 1e-6) ops.push(a / b);
                for (let res of ops) {
                    if (solve([...next, res])) return true;
                }
            }
        }
        return false;
    };
    return solve(cards.map(Number));
};
