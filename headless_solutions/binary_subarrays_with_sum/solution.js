/**
 * @param {number[]} nums
 * @param {number} goal
 * @return {number}
 */
var numSubarraysWithSum = function(nums, goal) {
    let c = new Map([[0, 1]]);
    let psum = res = 0;
    for (let i of nums) {
        psum += i;
        res += c.get(psum - goal);
        c.set(psum, (c.get(psum) || 0) + 1);
    }
    return res;
};
