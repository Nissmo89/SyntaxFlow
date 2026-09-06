/**
 * @param {number[]} arr
 * @return {number}
 */
var maxTurbulenceSize = function(arr) {
    let inc = 1, dec = 1, result = 1;
    for (let i = 1; i < arr.length; i++) {
        if (arr[i] > arr[i - 1]) {
            inc = dec + 1;
            dec = 1;
        } else if (arr[i] < arr[i - 1]) {
            dec = inc + 1;
            inc = 1;
        } else {
            inc = 1;
            dec = 1;
        }
        result = Math.max(result, inc, dec);
    }
    return result;
};
