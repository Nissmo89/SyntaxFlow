var maxSatisfied = function(customers, grumpy, minutes) {
    let totalSatisfied = 0;
    let currentWindowGain = 0;
    let maxWindowGain = 0;

    for (let i = 0; i < customers.length; i++) {
        if (grumpy[i] === 0) totalSatisfied += customers[i];
        else currentWindowGain += customers[i];

        if (i >= minutes && grumpy[i - minutes] === 1) {
            currentWindowGain -= customers[i - minutes];
        }
        maxWindowGain = Math.max(maxWindowGain, currentWindowGain);
    }
    return totalSatisfied + maxWindowGain;
};
