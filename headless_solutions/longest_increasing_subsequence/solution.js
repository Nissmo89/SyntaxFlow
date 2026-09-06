/**
 * @param {number[]} nums
 * @return {number}
 */
var lengthOfLIS = function(nums) {
    let tails = [];
    for (let x of nums) {
        let i = 0, j = tails.length;
        while (i < j) {
            let m = Math.floor((i + j) / 2);
            if (tails[m] < x) i = m + 1;
            else j = m;
        }
        if (i === tails.length) tails.push(x);
        else tails[i] = x;
    }
    return tails.length;
};
