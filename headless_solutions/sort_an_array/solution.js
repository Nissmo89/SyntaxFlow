var sortArray = function(nums) {
    if (nums.length <= 1) return nums;
    const mid = Math.floor(nums.length / 2);
    const left = sortArray(nums.slice(0, mid));
    const right = sortArray(nums.slice(mid));
    
    let res = [], i = 0, j = 0;
    while (i < left.length && j < right.length) {
        res.push(left[i] <= right[j] ? left[i++] : right[j++]);
    }
    return res.concat(left.slice(i)).concat(right.slice(j));
};
