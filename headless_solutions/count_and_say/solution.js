/**
 * @param {number} n
 * @return {string}
 */
var countAndSay = function(n) {
    let res = "1";
    for (let i = 1; i < n; i++) {
        let next = "";
        for (let j = 0; j < res.length; j++) {
            let count = 1;
            while (j + 1 < res.length && res[j] === res[j + 1]) {
                count++;
                j++;
            }
            next += count.toString() + res[j];
        }
        res = next;
    }
    return res;
};
