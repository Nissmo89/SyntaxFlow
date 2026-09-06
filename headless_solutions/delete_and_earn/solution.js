/**
 * @param {number[]} nums
 * @return {number}
 */
var deleteAndEarn = function(nums) {
    let maxVal = Math.max(...nums);
    let points = new Array(maxVal + 1).fill(0);
    for (let num of nums) points[num] += num;
    
    let take = 0, skip = 0;
    for (let val of points) {
        let newTake = skip + val;
        let newSkip = Math.max(skip, take);
        take = newTake;
        skip = newSkip;
    }
    return Math.max(take, skip);
};
