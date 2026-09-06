/**
 * @param {number[]} heights
 * @return {number}
 */
var heightChecker = function(heights) {
    let count = new Array(101).fill(0);
    for (let height of heights) {
        count[height]++;
    }
    let res = 0;
    let h_ptr = 1;
    for (let height of heights) {
        while (count[h_ptr] == 0) {
            h_ptr++;
        }
        if (h_ptr != height) {
            res++;
        }
        count[h_ptr]--;
    }
    return res;
};
