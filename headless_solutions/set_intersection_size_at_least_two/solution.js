/**
 * @param {number[][]} intervals
 * @return {number}
 */
var intersectionSizeTwo = function(intervals) {
    intervals.sort((a, b) => a[1] !== b[1] ? a[1] - b[1] : b[0] - a[0]);
    let p1 = -1, p2 = -1, ans = 0;
    for (const [s, e] of intervals) {
        if (s > p2) {
            ans += 2;
            p1 = e - 1;
            p2 = e;
        } else if (s > p1) {
            ans += 1;
            p1 = p2;
            p2 = e;
        }
    }
    return ans;
};
