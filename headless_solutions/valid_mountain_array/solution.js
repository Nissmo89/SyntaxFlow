/**
 * @param {number[]} arr
 * @return {boolean}
 */
var validMountainArray = function(arr) {
    let n = arr.length;
    let l = 0, r = n - 1;
    while (l + 1 < n && arr[l] < arr[l + 1]) l++;
    while (r > 0 && arr[r - 1] > arr[r]) r--;
    return 0 < l && l == r && r < n - 1;
};
