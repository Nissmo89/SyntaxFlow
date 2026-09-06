/**
 * @param {number[]} nums1
 * @param {number[]} nums2
 * @return {number}
 */
var minSwap = function(nums1, nums2) {
    const n = nums1.length;
    let keep = 0;
    let swap = 1;

    for (let i = 1; i < n; i++) {
        let newKeep = Infinity;
        let newSwap = Infinity;

        // Case 1: Natural order is valid
        if (nums1[i - 1] < nums1[i] && nums2[i - 1] < nums2[i]) {
            newKeep = Math.min(newKeep, keep);
            newSwap = Math.min(newSwap, swap + 1);
        }

        // Case 2: Crossed order is valid
        if (nums1[i - 1] < nums2[i] && nums2[i - 1] < nums1[i]) {
            newKeep = Math.min(newKeep, swap);
            newSwap = Math.min(newSwap, keep + 1);
        }

        keep = newKeep;
        swap = newSwap;
    }

    return Math.min(keep, swap);
};
