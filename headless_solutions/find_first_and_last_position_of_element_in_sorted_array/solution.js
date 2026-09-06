var searchRange = function(nums, target) {
    const findBound = (isFirst) => {
        let low = 0, high = nums.length - 1, ans = -1;
        while (low <= high) {
            let mid = Math.floor((low + high) / 2);
            if (nums[mid] === target) {
                ans = mid;
                if (isFirst) high = mid - 1;
                else low = mid + 1;
            } else if (nums[mid] < target) low = mid + 1;
            else high = mid - 1;
        }
        return ans;
    };
    return [findBound(true), findBound(false)];
};
