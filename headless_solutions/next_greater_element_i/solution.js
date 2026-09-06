/**
 * @param {number[]} nums1
 * @param {number[]} nums2
 * @return {number[]}
 */
var nextGreaterElement = function(nums1, nums2) {
    let stack = [];
    let m = {};
    for (let n of nums2) {
        while (stack.length && stack[stack.length - 1] < n) {
            m[stack.pop()] = n;
        }
        stack.push(n);
    }
    return nums1.map(n => m[n] || -1);
};
