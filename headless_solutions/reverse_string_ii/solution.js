/**
 * @param {string} s
 * @param {number} k
 * @return {string}
 */
var reverseStr = function(s, k) {
    let arr = s.split("");
    for (let i = 0; i < arr.length; i += 2 * k) {
        let j = Math.min(i + k - 1, arr.length - 1);
        reverse(arr, i, j);
    }
    return arr.join("");
};
function reverse(arr, l, r) {
    while (l < r) {
        let temp = arr[l];
        arr[l++] = arr[r];
        arr[r--] = temp;
    }
}
