/**
 * @param {number[][]} arrays
 * @return {number}
 */
var maxDistance = function(arrays) {
    let minVal = arrays[0][0];
    let maxVal = arrays[0][arrays[0].length - 1];
    let maxDist = 0;
    
    for (let i = 1; i < arrays.length; i++) {
        const arr = arrays[i];
        maxDist = Math.max(maxDist, Math.abs(arr[arr.length - 1] - minVal), Math.abs(maxVal - arr[0]));
        minVal = Math.min(minVal, arr[0]);
        maxVal = Math.max(maxVal, arr[arr.length - 1]);
    }
    return maxDist;
};
