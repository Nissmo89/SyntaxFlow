/**
 * @param {number[]} nums
 * @return {string}
 */
var optimalDivision = function(nums) {
    const n = nums.length;
    if (n === 1) return nums[0].toString();
    if (n === 2) return `${nums[0]}/${nums[1]}`;
    
    let res = `${nums[0]}/(${nums[1]}`;
    for (let i = 2; i < n; i++) {
        res += `/${nums[i]}`;
    }
    res += ")";
    return res;
};
