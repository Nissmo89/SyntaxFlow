/**
 * @param {number[]} nums
 * @return {number[]}
 */
var countSmaller = function(nums) {
    const n = nums.length;
    const counts = new Array(n).fill(0);
    const indices = Array.from({length: n}, (_, i) => i);

    function mergeSort(left, right) {
        if (left >= right) return;
        const mid = Math.floor((left + right) / 2);
        mergeSort(left, mid);
        mergeSort(mid + 1, right);
        
        const temp = [];
        let i = left, j = mid + 1;
        while (i <= mid && j <= right) {
            if (nums[indices[i]] > nums[indices[j]]) {
                counts[indices[i]] += (right - j + 1);
                temp.push(indices[i++]);
            } else {
                temp.push(indices[j++]);
            }
        }
        while (i <= mid) temp.push(indices[i++]);
        while (j <= right) temp.push(indices[j++]);
        for (let k = 0; k < temp.length; k++) indices[left + k] = temp[k];
    }

    mergeSort(0, n - 1);
    return counts;
};
