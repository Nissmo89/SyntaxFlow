/**
 * @param {number[]} nums
 * @return {number}
 */
var countTriplets = function(nums) {
    const count = new Int32Array(65536);
    for (let a of nums) {
        for (let b of nums) {
            count[a & b]++;
        }
    }
    let ans = 0;
    for (let a of nums) {
        for (let i = 0; i < 65536; i++) {
            if ((a & i) === 0) {
                ans += count[i];
            }
        }
    }
    return ans;
};
