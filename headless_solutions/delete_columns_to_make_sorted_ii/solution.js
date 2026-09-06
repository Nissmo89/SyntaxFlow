var minDeletionSize = function(strs) {
    const n = strs.length, m = strs[0].length;
    let deletions = 0;
    let isSorted = new Array(n - 1).fill(false);
    
    for (let j = 0; j < m; j++) {
        let mustDelete = false;
        for (let i = 0; i < n - 1; i++) {
            if (!isSorted[i] && strs[i][j] > strs[i+1][j]) {
                mustDelete = true;
                break;
            }
        }
        if (mustDelete) {
            deletions++;
        } else {
            for (let i = 0; i < n - 1; i++) {
                if (strs[i][j] < strs[i+1][j]) isSorted[i] = true;
            }
        }
    }
    return deletions;
};
