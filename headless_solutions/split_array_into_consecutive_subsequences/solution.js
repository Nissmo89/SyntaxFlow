/**
 * @param {number[]} nums
 * @return {boolean}
 */
var isPossible = function(nums) {
    let left = new Map(), end = new Map();
    for (let x of nums) left.set(x, (left.get(x) || 0) + 1);
    for (let x of nums) {
        if (left.get(x) === 0) continue;
        left.set(x, left.get(x) - 1);
        if ((end.get(x - 1) || 0) > 0) {
            end.set(x - 1, end.get(x - 1) - 1);
            end.set(x, (end.get(x) || 0) + 1);
        } else if ((left.get(x + 1) || 0) > 0 && (left.get(x + 2) || 0) > 0) {
            left.set(x + 1, left.get(x + 1) - 1);
            left.set(x + 2, left.get(x + 2) - 1);
            end.set(x + 2, (end.get(x + 2) || 0) + 1);
        } else return false;
    }
    return true;
};
