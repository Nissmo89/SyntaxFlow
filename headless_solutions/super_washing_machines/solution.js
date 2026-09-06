var findMinMoves = function(machines) {
    const total = machines.reduce((a, b) => a + b, 0);
    if (total % machines.length !== 0) return -1;
    
    const target = total / machines.length;
    let balance = 0, maxMoves = 0;
    
    for (const load of machines) {
        balance += (load - target);
        maxMoves = Math.max(maxMoves, Math.abs(balance), load - target);
    }
    return maxMoves;
};
