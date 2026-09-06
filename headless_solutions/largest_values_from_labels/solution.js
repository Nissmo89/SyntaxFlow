var largestValsFromLabels = function(values, labels, numWanted, useLimit) {
    let items = values.map((v, i) => [v, labels[i]]);
    items.sort((a, b) => b[0] - a[0]);
    
    let labelCounts = new Map();
    let totalSum = 0, count = 0;
    
    for (let [val, label] of items) {
        if (count === numWanted) break;
        let currentLabelCount = labelCounts.get(label) || 0;
        if (currentLabelCount < useLimit) {
            totalSum += val;
            labelCounts.set(label, currentLabelCount + 1);
            count++;
        }
    }
    return totalSum;
};
