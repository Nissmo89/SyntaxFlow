/**
 * @param {number[]} arr
 * @return {number}
 */
var maxChunksToSorted = function(arr) {
    const n = arr.length;
    const minOfRight = new Array(n);
    
    // Build suffix minimum array
    minOfRight[n - 1] = arr[n - 1];
    for (let i = n - 2; i >= 0; i--) {
        minOfRight[i] = Math.min(arr[i], minOfRight[i + 1]);
    }

    let maxOfLeft = arr[0];
    let cuts = 0;

    // Check each possible split point
    for (let i = 0; i < n - 1; i++) {
        maxOfLeft = Math.max(maxOfLeft, arr[i]);
        if (maxOfLeft <= minOfRight[i + 1]) {
            cuts++;
        }
    }

    return cuts + 1;
};
