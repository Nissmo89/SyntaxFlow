var findReplaceString = function(s, indices, sources, targets) {
    const map = new Map();
    for (let i = 0; i < indices.length; i++) {
        if (s.substring(indices[i], indices[i] + sources[i].length) === sources[i]) {
            map.set(indices[i], i);
        }
    }
    let res = "";
    for (let i = 0; i < s.length; ) {
        if (map.has(i)) {
            const idx = map.get(i);
            res += targets[idx];
            i += sources[idx].length;
        } else {
            res += s[i++];
        }
    }
    return res;
};
