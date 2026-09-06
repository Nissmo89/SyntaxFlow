/**
 * @param {number[][]} intervals
 * @return {number[]}
 */
var findRightInterval = function(intervals) {
    const n = intervals.length;
    const starts = intervals.map((v, i) => [v[0], i]).sort((a, b) => a[0] - b[0]);
    
    return intervals.map(interval => {
        const target = interval[1];
        let left = 0, right = n - 1, ans = -1;
        while (left <= right) {
            let mid = Math.floor((left + right) / 2);
            if (starts[mid][0] >= target) {
                ans = starts[mid][1];
                right = mid - 1;
            } else {
                left = mid + 1;
            }
        }
        return ans;
    });
};
