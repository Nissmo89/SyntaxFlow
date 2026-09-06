var findFrequentTreeSum = function(root) {
    const counts = new Map();
    let maxFreq = 0;

    function dfs(node) {
        if (!node) return 0;
        const sum = node.val + dfs(node.left) + dfs(node.right);
        const freq = (counts.get(sum) || 0) + 1;
        counts.set(sum, freq);
        maxFreq = Math.max(maxFreq, freq);
        return sum;
    }

    dfs(root);
    const res = [];
    for (let [sum, freq] of counts) {
        if (freq === maxFreq) res.push(sum);
    }
    return res;
};
