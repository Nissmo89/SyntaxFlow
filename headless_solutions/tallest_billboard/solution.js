/**
 * @param {number[]} rods
 * @return {number}
 */
var tallestBillboard = function(rods) {
    let dp = new Map();
    dp.set(0, 0);
    
    for (let r of rods) {
        let next = new Map(dp);
        for (let [diff, h] of dp) {
            next.set(diff + r, Math.max(next.get(diff + r) || 0, h));
            let newDiff = Math.abs(diff - r);
            next.set(newDiff, Math.max(next.get(newDiff) || 0, h + Math.min(diff, r)));
        }
        dp = next;
    }
    return dp.get(0);
};
