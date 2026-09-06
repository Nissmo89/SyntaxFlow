/**
 * @param {number[]} gas
 * @param {number[]} cost
 * @return {number}
 */
var canCompleteCircuit = function(gas, cost) {
    let totalSurplus = 0, currentTank = 0, startIndex = 0;
    for (let i = 0; i < gas.length; i++) {
        let net = gas[i] - cost[i];
        totalSurplus += net;
        currentTank += net;
        if (currentTank < 0) {
            startIndex = i + 1;
            currentTank = 0;
        }
    }
    return totalSurplus < 0 ? -1 : startIndex;
};
