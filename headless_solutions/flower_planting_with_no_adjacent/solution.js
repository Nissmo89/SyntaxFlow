/**
 * @param {number} n
 * @param {number[][]} paths
 * @return {number[]}
 */
var gardenNoAdj = function(n, paths) {
    const adj = Array.from({ length: n }, () => []);
    for (const [u, v] of paths) {
        adj[u - 1].push(v - 1);
        adj[v - 1].push(u - 1);
    }
    const answer = new Array(n).fill(0);
    for (let i = 0; i < n; i++) {
        const used = new Set();
        for (const neighbor of adj[i]) {
            if (answer[neighbor] !== 0) used.add(answer[neighbor]);
        }
        for (let color = 1; color <= 4; color++) {
            if (!used.has(color)) {
                answer[i] = color;
                break;
            }
        }
    }
    return answer;
};
