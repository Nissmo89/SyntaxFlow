/**
 * @param {number[]} nums
 * @return {number}
 */
var maximumProduct = function(nums) {
    let max1 = -Infinity, max2 = -Infinity, max3 = -Infinity, min1 = Infinity, min2 = Infinity;
    for (let n of nums) {
        if (n > max1) {
            max3 = max2;
            max2 = max1;
            max1 = n;
        } else if (n > max2) {
            max3 = max2;
            max2 = n;
        } else if (n > max3) {
            max3 = n;
        }

        if (n < min1) {
            min2 = min1;
            min1 = n;
        } else if (n < min2) {
            min2 = n;
        }
    }
    return Math.max(max1*max2*max3, max1*min1*min2);
};
