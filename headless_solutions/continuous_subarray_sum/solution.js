/**
 * @param {number[]} nums
 * @param {number} k
 * @return {boolean}
 */
var checkSubarraySum = function(nums, k) {
    // Map to store {remainder => first_seen_index}
    const seen = new Map();
    seen.set(0, -1);
    
    let prefixMod = 0;
    
    for (let i = 0; i < nums.length; i++) {
        prefixMod = (prefixMod + nums[i]) % k;
        
        if (seen.has(prefixMod)) {
            if (i - seen.get(prefixMod) >= 2) {
                return true;
            }
        } else {
            // Keep the earliest index
            seen.set(prefixMod, i);
        }
    }
    
    return false;
};
