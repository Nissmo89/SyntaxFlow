/**
 * @param {number[]} nums
 */
var Solution = function(nums) {
    this.indexMap = new Map();
    
    for (let i = 0; i < nums.length; i++) {
        if (!this.indexMap.has(nums[i])) {
            this.indexMap.set(nums[i], []);
        }
        this.indexMap.get(nums[i]).push(i);
    }
};

/** 
 * @param {number} target
 * @return {number}
 */
Solution.prototype.pick = function(target) {
    const indices = this.indexMap.get(target);
    const randomIndex = Math.floor(Math.random() * indices.length);
    return indices[randomIndex];
};

/** 
 * Your Solution object will be instantiated and called as such:
 * var obj = new Solution(nums)
 * var param_1 = obj.pick(target)
 */
