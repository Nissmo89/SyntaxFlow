/**
 * @param {number[]} nums
 * @return {number}
 */
var findShortestSubArray = function(nums) {
    // Map to store the first occurrence index of each number
    const leftIdx = new Map();
    // Map to store the last occurrence index of each number
    const rightIdx = new Map();
    // Map to store the frequency (count) of each number
    const counts = new Map();

    // First pass: Populate the maps
    for (let i = 0; i < nums.length; i++) {
        const num = nums[i];
        
        // If num is not in leftIdx, this is its first occurrence
        if (!leftIdx.has(num)) {
            leftIdx.set(num, i);
        }
        
        // Always update rightIdx to store the latest (last) occurrence
        rightIdx.set(num, i);
        
        // Increment the count for num
        counts.set(num, (counts.get(num) || 0) + 1);
    }

    // Find the maximum frequency (degree of the array)
    let degree = 0;
    // Iterate over map values to find the maximum
    for (const count of counts.values()) {
        degree = Math.max(degree, count);
    }

    // Initialize minLen with the maximum possible length (length of nums itself)
    let minLen = nums.length;

    // Second pass: Find the shortest subarray with the same degree
    // Iterate over map entries (key-value pairs)
    for (const [num, count] of counts.entries()) {
        // If this number's frequency matches the array's degree
        if (count === degree) {
            // Calculate the length of the subarray containing all occurrences of num
            const currentLen = rightIdx.get(num) - leftIdx.get(num) + 1;
            // Update minLen if this subarray is shorter
            minLen = Math.min(minLen, currentLen);
        }
    }
    
    return minLen;
};
