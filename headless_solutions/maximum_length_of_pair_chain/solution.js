/**
 * @param {number[][]} pairs
 * @return {number}
 */
var findLongestChain = function(pairs) {
    // Sort by the second element
    pairs.sort((a, b) => a[1] - b[1]);
    
    let count = 0;
    let currentEnd = -Infinity;
    
    for (const [left, right] of pairs) {
        if (left > currentEnd) {
            currentEnd = right;
            count++;
        }
    }
    return count;
};
