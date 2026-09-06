/**
 * @param {number[]} candidates
 * @param {number} target
 * @return {number[][]}
 */
var combinationSum2 = function(candidates, target) {
    candidates.sort((a, b) => a - b);
    const result = [];
    const path = [];

    function backtrack(start, remain, pathSum) {
        if (pathSum === remain) {
            result.push(path.slice());
            return;
        }
        if (pathSum > remain) {
            return;
        }
        for (let i = start; i < candidates.length; i++) {
            if (i > start && candidates[i] === candidates[i - 1]) continue; // skip duplicates
            path.push(candidates[i]);
            backtrack(i + 1, remain, pathSum + candidates[i]);
            path.pop();
        }
    }

    backtrack(0, target, 0);
    return result;
};
