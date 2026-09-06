/**
 * @param {number[][]} nums
 * @return {number[]}
 */
var smallestRange = function(nums) {
    const pq = new MinPriorityQueue({ priority: x => x.val });
    let curMax = -Infinity;
    for (let i = 0; i < nums.length; i++) {
        pq.enqueue({ val: nums[i][0], r: i, c: 0 });
        curMax = Math.max(curMax, nums[i][0]);
    }
    let ans = [-1e6, 1e6];
    while (pq.size() === nums.length) {
        const { val: minVal, r, c } = pq.dequeue().element;
        if (curMax - minVal < ans[1] - ans[0]) ans = [minVal, curMax];
        if (c + 1 < nums[r].length) {
            const nextVal = nums[r][c + 1];
            pq.enqueue({ val: nextVal, r, c: c + 1 });
            curMax = Math.max(curMax, nextVal);
        } else break;
    }
    return ans;
};
