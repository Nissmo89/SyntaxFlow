var numSimilarGroups = function(strs) {
    const n = strs.length;
    const parent = Array.from({length: n}, (_, i) => i);
    let count = n;

    const find = (i) => {
        if (parent[i] === i) return i;
        return parent[i] = find(parent[i]);
    };

    const union = (i, j) => {
        let rootI = find(i), rootJ = find(j);
        if (rootI !== rootJ) {
            parent[rootI] = rootJ;
            count--;
        }
    };

    const isSimilar = (s1, s2) => {
        let diff = 0;
        for (let i = 0; i < s1.length; i++) {
            if (s1[i] !== s2[i] && ++diff > 2) return false;
        }
        return true;
    };

    for (let i = 0; i < n; i++) {
        for (let j = i + 1; j < n; j++) {
            if (isSimilar(strs[i], strs[j])) union(i, j);
        }
    }
    return count;
};
