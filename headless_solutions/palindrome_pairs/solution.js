var palindromePairs = function(words) {
    const map = new Map();
    words.forEach((w, i) => map.set(w, i));
    const res = [];
    const isPal = (s) => {
        for (let i = 0; i < s.length >> 1; i++) if (s[i] !== s[s.length - 1 - i]) return false;
        return true;
    };
    for (let i = 0; i < words.length; i++) {
        for (let j = 0; j <= words[i].length; j++) {
            const left = words[i].slice(0, j), right = words[i].slice(j);
            if (isPal(left)) {
                const revRight = right.split('').reverse().join('');
                if (map.has(revRight) && map.get(revRight) !== i) res.push([map.get(revRight), i]);
            }
            if (j !== words[i].length && isPal(right)) {
                const revLeft = left.split('').reverse().join('');
                if (map.has(revLeft) && map.get(revLeft) !== i) res.push([i, map.get(revLeft)]);
            }
        }
    }
    return res;
};
