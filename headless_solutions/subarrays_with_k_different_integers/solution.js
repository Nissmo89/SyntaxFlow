var subarraysWithKDistinct = function(nums, k) {
    const atMostK = (k) => {
        let left = 0, res = 0;
        let map = new Map();
        for (let right = 0; right < nums.length; right++) {
            map.set(nums[right], (map.get(nums[right]) || 0) + 1);
            while (map.size > k) {
                map.set(nums[left], map.get(nums[left]) - 1);
                if (map.get(nums[left]) === 0) map.delete(nums[left]);
                left++;
            }
            res += (right - left + 1);
        }
        return res;
    };
    return atMostK(k) - atMostK(k - 1);
};
