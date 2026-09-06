/**
 * @param {number[]} nums
 * @return {number}
 */
var bestRotation = function(nums) {
    const n = nums.length;
    const change = new Array(n).fill(0);
    for (let i = 0; i < n; i++) {
        change[(i - nums[i] + 1 + n) % n]--;
    }
    let maxScore = -1, bestK = 0, currentScore = 0;
    for (let k = 0; k < n; k++) {
        currentScore += change[k] + 1;
        if (currentScore > maxScore) {
            maxScore = currentScore;
            bestK = k;
        }
    }
    return bestK;
};
