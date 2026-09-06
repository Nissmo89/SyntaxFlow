/**
 * @param {number[]} nums1
 * @param {number[]} nums2
 * @return {number[]}
 */
var intersection = function(nums1, nums2) {
    let m = new Set(nums1);
    let res = [];
    for (let a of nums2) {
        if (m.has(a)) {
            res.push(a);
            m.delete(a);
        }
    }
    return res;
};
