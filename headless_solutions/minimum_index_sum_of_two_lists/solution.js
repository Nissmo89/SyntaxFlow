/**
 * @param {string[]} list1
 * @param {string[]} list2
 * @return {string[]}
 */
var findRestaurant = function(list1, list2) {
    let map = {};
    for (let i = 0; i < list1.length; i++) {
        map[list1[i]] = i;
    }
    let minSum = Infinity;
    let res = [];
    for (let i = 0; i < list2.length; i++) {
        if (map[list2[i]]) {
            let sum = map[list2[i]] + i;
            if (sum < minSum) {
                minSum = sum;
                res = [list2[i]];
            } else if (sum == minSum) {
                res.push(list2[i]);
            }
        }
    }
    return res;
};
