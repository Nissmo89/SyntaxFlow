/**
 * @param {number[]} arr
 * @return {boolean}
 */
var canThreePartsEqualSum = function(arr) {
    const totalSum = arr.reduce((a, b) => a + b, 0);
    if (totalSum % 3 !== 0) return false;
    
    const target = totalSum / 3;
    let currentSum = 0, count = 0;
    
    for (let i = 0; i < arr.length - 1; i++) {
        currentSum += arr[i];
        if (currentSum === target) {
            count++;
            currentSum = 0;
            if (count === 2) return true;
        }
    }
    return false;
};
