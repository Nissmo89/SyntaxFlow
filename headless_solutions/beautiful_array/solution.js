/**
 * @param {number} n
 * @return {number[]}
 */
var beautifulArray = function(n) {
    let res = [1];
    while (res.length < n) {
        let tmp = [];
        for (let i of res) {
            if (i * 2 - 1 <= n) {
                tmp.push(i * 2 - 1);
            }
            if (i * 2 <= n) {
                tmp.push(i * 2);
            }
        }
        res = tmp;
    }
    return res;
};
