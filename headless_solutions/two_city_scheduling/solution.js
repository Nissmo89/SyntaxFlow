/**
 * @param {number[][]} costs
 * @return {number}
 */
var twoCitySchedCost = function(costs) {
    // Sort by difference (costB - costA)
    costs.sort((a, b) => (a[1] - a[0]) - (b[1] - b[0]));
    
    const n = costs.length / 2;
    let totalCost = 0;
    for (let i = 0; i < n; i++) totalCost += costs[i][1];
    for (let i = n; i < 2 * n; i++) totalCost += costs[i][0];
    return totalCost;
};
