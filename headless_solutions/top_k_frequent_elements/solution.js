/**
 * @param {number[]} nums
 * @param {number} k
 * @return {number[]}
 */
var topKFrequent = function(nums, k) {
    const map = new Map();
    for (const n of nums) map.set(n, (map.get(n) || 0) + 1);
    
    const buckets = Array.from({ length: nums.length + 1 }, () => []);
    for (const [num, freq] of map.entries()) {
        buckets[freq].push(num);
    }
    
    const res = [];
    for (let i = buckets.length - 1; i >= 0 && res.length < k; i--) {
        for (const num of buckets[i]) {
            res.push(num);
            if (res.length === k) break;
        }
    }
    return res;
};
