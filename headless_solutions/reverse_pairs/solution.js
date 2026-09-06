var reversePairs = function(nums) {
    const mergeSort = (l, r) => {
        if (l >= r) return 0;
        let mid = Math.floor((l + r) / 2);
        let count = mergeSort(l, mid) + mergeSort(mid + 1, r);
        let j = mid + 1;
        for (let i = l; i <= mid; i++) {
            while (j <= r && nums[i] > 2 * nums[j]) j++;
            count += (j - (mid + 1));
        }
        let sorted = [], i = l, k = mid + 1;
        while (i <= mid || k <= r) {
            if (i > mid) sorted.push(nums[k++]);
            else if (k > r) sorted.push(nums[i++]);
            else if (nums[i] <= nums[k]) sorted.push(nums[i++]);
            else sorted.push(nums[k++]);
        }
        for (let i = 0; i < sorted.length; i++) nums[l + i] = sorted[i];
        return count;
    };
    return mergeSort(0, nums.length - 1);
};
