/**
 * @param {number} n
 * @param {number[][]} edges
 * @return {number[]}
 */
var sumOfDistancesInTree = function(n, edges) {
    const adj = Array.from({ length: n }, () => []);
    for (const [u, v] of edges) {
        adj[u].push(v);
        adj[v].push(u);
    }
    const count = new Array(n).fill(1);
    const res = new Array(n).fill(0);

    function dfs1(u, p) {
        for (const v of adj[u]) {
            if (v === p) continue;
            dfs1(v, u);
            count[u] += count[v];
            res[0] += count[v];
        }
    }

    function dfs2(u, p) {
        for (const v of adj[u]) {
            if (v === p) continue;
            res[v] = res[u] - count[v] + (n - count[v]);
            dfs2(v, u);
        }
    }

    dfs1(0, -1);
    dfs2(0, -1);
    return res;
};
