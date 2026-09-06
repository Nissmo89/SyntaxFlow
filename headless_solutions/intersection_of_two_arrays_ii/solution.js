/**
 * @param {number[]} nums1
 * @param {number[]} nums2
 * @return {number[]}
 */
var intersect = function(nums1, nums2) {
    if (nums1.length > nums2.length) {
        return intersect(nums2, nums1);
    }

    let freq = {};
    for (let num of nums1) {
        freq[num] = (freq[num] || 0) + 1;
    }

    let ans = [];
    for (let num of nums2) {
        if (freq[num] > 0) {
            ans.push(num);
            freq[num]--;
        }
    }

    return ans;
};
