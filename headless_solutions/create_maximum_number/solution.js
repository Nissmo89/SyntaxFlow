/**
 * @param {number[]} nums1
 * @param {number[]} nums2
 * @param {number} k
 * @return {number[]}
 */
var maxNumber = function(nums1, nums2, k) {
    function maxSubsequence(nums, k) {
        let stack = [];
        let drop = nums.length - k;
        for (let num of nums) {
            while (drop > 0 && stack.length && stack[stack.length - 1] < num) {
                stack.pop();
                drop--;
            }
            stack.push(num);
        }
        return stack.slice(0, k);
    }

    function greater(a, i, b, j) {
        while (i < a.length && j < b.length && a[i] === b[j]) {
            i++; j++;
        }
        return j === b.length || (i < a.length && a[i] > b[j]);
    }

    function merge(a, b) {
        let res = [];
        let i = 0, j = 0;
        while (i < a.length || j < b.length) {
            if (greater(a, i, b, j)) {
                res.push(a[i++]);
            } else {
                res.push(b[j++]);
            }
        }
        return res;
    }

    let best = [];
    let start = Math.max(0, k - nums2.length);
    let end = Math.min(k, nums1.length);
    for (let i = start; i <= end; i++) {
        let sub1 = maxSubsequence(nums1, i);
        let sub2 = maxSubsequence(nums2, k - i);
        let merged = merge(sub1, sub2);
        if (best.length === 0 || greater(merged, 0, best, 0)) {
            best = merged;
        }
    }
    return best;
};
