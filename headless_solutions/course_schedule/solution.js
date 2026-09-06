var canFinish = function(numCourses, prerequisites) {
    const adj = Array.from({ length: numCourses }, () => []);
    const inDegree = new Array(numCourses).fill(0);
    for (const [dest, src] of prerequisites) {
        adj[src].push(dest);
        inDegree[dest]++;
    }
    const queue = [];
    for (let i = 0; i < numCourses; i++) if (inDegree[i] === 0) queue.push(i);
    let count = 0;
    while (queue.length > 0) {
        const u = queue.shift();
        count++;
        for (const v of adj[u]) {
            if (--inDegree[v] === 0) queue.push(v);
        }
    }
    return count === numCourses;
};
