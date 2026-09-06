/**
 * @param {number[]} arr
 * @return {number[]}
 */
var pancakeSort = function(arr) {
    const res = [];
    for (let target = arr.length; target > 0; target--) {
        let i = arr.indexOf(target);
        if (i === target - 1) continue;
        if (i > 0) {
            res.push(i + 1);
            reverse(arr, i + 1);
        }
        res.push(target);
        reverse(arr, target);
    }
    return res;
};

function reverse(arr, k) {
    let i = 0, j = k - 1;
    while (i < j) {
        [arr[i], arr[j]] = [arr[j], arr[i]];
        i++; j--;
    }
}
