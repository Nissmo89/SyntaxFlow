var findLadders = function(beginWord, endWord, wordList) {
    const dict = new Set(wordList);
    if (!dict.has(endWord)) return [];
    const dist = new Map([[beginWord, 0]]);
    const adj = new Map();
    const queue = [beginWord];
    
    while (queue.length) {
        const curr = queue.shift();
        for (let i = 0; i < curr.length; i++) {
            for (let j = 0; j < 26; j++) {
                const next = curr.slice(0, i) + String.fromCharCode(97 + j) + curr.slice(i + 1);
                if (dict.has(next)) {
                    if (!dist.has(next)) {
                        dist.set(next, dist.get(curr) + 1);
                        queue.push(next);
                        adj.set(curr, [...(adj.get(curr) || []), next]);
                    } else if (dist.get(next) === dist.get(curr) + 1) {
                        adj.set(curr, [...(adj.get(curr) || []), next]);
                    }
                }
            }
        }
    }
    const res = [];
    const dfs = (curr, path) => {
        if (curr === endWord) { res.push([...path]); return; }
        for (const neighbor of (adj.get(curr) || [])) {
            path.push(neighbor);
            dfs(neighbor, path);
            path.pop();
        }
    };
    dfs(beginWord, [beginWord]);
    return res;
};
