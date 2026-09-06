var maximumGap = function(nums) {
    const n = nums.length;
    if (n < 2) return 0;
    let minVal = Math.min(...nums), maxVal = Math.max(...nums);
    if (minVal === maxVal) return 0;

    const bucketSize = Math.max(1, Math.floor((maxVal - minVal) / (n - 1)));
    const numBuckets = Math.floor((maxVal - minVal) / bucketSize) + 1;
    const minB = new Array(numBuckets).fill(Infinity);
    const maxB = new Array(numBuckets).fill(-1);

    for (let x of nums) {
        let idx = Math.floor((x - minVal) / bucketSize);
        minB[idx] = Math.min(minB[idx], x);
        maxB[idx] = Math.max(maxB[idx], x);
    }

    let maxGap = 0, prevMax = minVal;
    for (let i = 0; i < numBuckets; i++) {
        if (maxB[i] === -1) continue;
        maxGap = Math.max(maxGap, minB[i] - prevMax);
        prevMax = maxB[i];
    }
    return maxGap;
};
