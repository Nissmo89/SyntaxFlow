/**
 * @param {number[]} arr
 * @return {number}
 */
var longestMountain = function(arr) {
    let maxLen = 0;
    for (let i = 1; i < arr.length - 1; i++) {
        if (arr[i] > arr[i - 1] && arr[i] > arr[i + 1]) {
            let left = i - 1, right = i + 1;
            while (left > 0 && arr[left - 1] < arr[left]) left--;
            while (right < arr.length - 1 && arr[right] > arr[right + 1]) right++;
            maxLen = Math.max(maxLen, right - left + 1);
            i = right;
        }
    }
    return maxLen;
};
