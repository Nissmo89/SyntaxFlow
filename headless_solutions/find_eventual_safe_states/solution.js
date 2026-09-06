/**
 * @param {number[][]} graph
 * @return {number[]}
 */
var eventualSafeNodes = function(graph) {
    const n = graph.length;
    const revGraph = Array.from({ length: n }, () => []);
    const outDegree = new Array(n).fill(0);
    const queue = [];
    
    for (let i = 0; i < n; i++) {
        outDegree[i] = graph[i].length;
        if (outDegree[i] === 0) queue.push(i);
        for (const neighbor of graph[i]) {
            revGraph[neighbor].push(i);
        }
    }
    
    const safeNodes = [];
    let head = 0;
    while (head < queue.length) {
        const u = queue[head++];
        safeNodes.push(u);
        for (const v of revGraph[u]) {
            if (--outDegree[v] === 0) queue.push(v);
        }
    }
    return safeNodes.sort((a, b) => a - b);
};
