/**
 * @param {number[]} nums
 * @return {number}
 */
var numSquarefulPerms = function(nums) {
    const isSquare = (n) => {
        const r = Math.floor(Math.sqrt(n));
        return r * r === n;
    };

    const count = new Map();
    for (const x of nums) {
        count.set(x, (count.get(x) || 0) + 1);
    }

    const graph = new Map();
    for (const x of count.keys()) {
        graph.set(x, []);
        for (const y of count.keys()) {
            if (isSquare(x + y)) {
                graph.get(x).push(y);
            }
        }
    }

    const dfs = (x, left) => {
        count.set(x, count.get(x) - 1);
        let ans = 0;
        
        if (left === 0) {
            ans = 1;
        } else {
            const neighbors = graph.get(x) || [];
            for (const y of neighbors) {
                if (count.get(y) > 0) {
                    ans += dfs(y, left - 1);
                }
            }
        }
        
        count.set(x, count.get(x) + 1); // Backtrack
        return ans;
    };

    let totalPermutations = 0;
    for (const x of count.keys()) {
        totalPermutations += dfs(x, nums.length - 1);
    }

    return totalPermutations;
};
