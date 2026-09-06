/**
 * @param {number[]} candidates
 * @param {number} target
 * @return {number[][]}
 */
var combinationSum = function(candidates, target) {
    candidates.sort((a, b) => a - b);
    const ans = [];
    const path = [];

    function backtrack(start, curSum) {
        if (curSum === target) {
            ans.push([...path]);
            return;
        }
        if (curSum > target) return;

        for (let i = start; i < candidates.length; i++) {
            if (curSum + candidates[i] > target) break; // pruning
            path.push(candidates[i]);
            backtrack(i, curSum + candidates[i]); // i allows reuse
            path.pop();
        }
    }

    backtrack(0, 0);
    return ans;
};
