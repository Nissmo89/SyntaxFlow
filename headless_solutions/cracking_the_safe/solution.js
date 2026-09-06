// JavaScript implementation matching signature
var crackSafe = function(n, k) {
    if (k === 1) return '0'.repeat(n);
    if (n === 1) return Array.from({length: k}, (_, i) => i).join('');

    const visited = new Set();
    const ans = [];
    const start = '0'.repeat(n - 1);

    const dfs = (node) => {
        for (let d = 0; d < k; d++) {
            const next = node.slice(1) + d;
            if (!visited.has(next)) {
                visited.add(next);
                dfs(next);
                ans.push(d);
            }
        }
    };

    dfs(start);
    ans.reverse();
    return '0'.repeat(n) + ans.join('');
};
