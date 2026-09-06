/**
 * @param {number[]} ages
 * @return {number}
 */
var numFriendRequests = function(ages) {
    let count = new Array(121).fill(0);
    for (let age of ages) count[age]++;
    
    let prefix = new Array(121).fill(0);
    for (let i = 1; i <= 120; i++) prefix[i] = prefix[i - 1] + count[i];
    
    let total = 0;
    for (let x = 1; x <= 120; x++) {
        if (count[x] === 0) continue;
        let min_y = Math.floor(x / 2) + 7 + 1;
        if (x < min_y) continue;
        
        let num_targets = prefix[x] - prefix[min_y - 1];
        num_targets--;
        total += num_targets * count[x];
    }
    return total;
};
