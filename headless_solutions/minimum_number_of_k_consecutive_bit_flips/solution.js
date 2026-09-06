/**
 * @param {number[]} nums
 * @param {number} k
 * @return {number}
 */
var minKBitFlips = function(nums, k) {
    const n = nums.length;
    let flipped = 0;
    let res = 0;

    for (let i = 0; i < n; i++) {
        // Expire flip outside current sliding window
        if (i >= k && nums[i - k] >= 2) {
            flipped ^= 1;
        }

        // Check if current effective bit is 0
        if (flipped === nums[i]) {
            if (i + k > n) {
                return -1;
            }
            nums[i] ^= 2; // Mark flip origin
            flipped ^= 1;
            res++;
        }
    }

    return res;
};
