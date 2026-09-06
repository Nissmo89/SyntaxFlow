var maxProfitAssignment = function(difficulty, profit, worker) {
    let jobs = difficulty.map((d, i) => [d, profit[i]]);
    jobs.sort((a, b) => a[0] - b[0]);
    worker.sort((a, b) => a - b);
    
    let totalProfit = 0, i = 0, bestProfit = 0;
    for (let ability of worker) {
        while (i < jobs.length && ability >= jobs[i][0]) {
            bestProfit = Math.max(bestProfit, jobs[i][1]);
            i++;
        }
        totalProfit += bestProfit;
    }
    return totalProfit;
};
