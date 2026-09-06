/**
 * @param {number[]} arr
 * @return {string}
 */
var largestTimeFromDigits = function(arr) {
    let ans = "";
    const permute = (arr, m = []) => {
        if (arr.length === 0) {
            const [h1, h2, m1, m2] = m;
            const h = h1 * 10 + h2;
            const min = m1 * 10 + m2;
            if (h < 24 && min < 60) {
                const time = `${h1}${h2}:${m1}${m2}`;
                if (time > ans) ans = time;
            }
        } else {
            for (let i = 0; i < arr.length; i++) {
                let curr = arr.slice();
                let next = curr.splice(i, 1);
                permute(curr, m.concat(next));
            }
        }
    };
    permute(arr);
    return ans;
};
