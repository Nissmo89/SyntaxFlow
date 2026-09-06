/**
 * @param {number[]} nums
 * @return {number}
 */
var findMaxLength = function(nums) {
    // Map to store the first occurrence of each count value.
    // Key: count, Value: index
    const prefixSumMap = new Map();
    
    // Initialize the map with count 0 at index -1.
    // This handles subarrays starting from index 0.
    prefixSumMap.set(0, -1);
    
    let maxLength = 0;
    let count = 0; // Running count: +1 for 1, -1 for 0
    
    for (let i = 0; i < nums.length; i++) {
        // Update count based on the current element
        if (nums[i] === 1) {
            count++;
        } else { // nums[i] === 0
            count--;
        }
        
        // Check if this count has been seen before
        if (prefixSumMap.has(count)) {
            // If seen, calculate the length of the subarray
            // from the previous occurrence of this count to the current index.
            const prevIndex = prefixSumMap.get(count);
            maxLength = Math.max(maxLength, i - prevIndex);
        } else {
            // If not seen, store the current index as the first occurrence
            prefixSumMap.set(count, i);
        }
    }
    
    return maxLength;
};
