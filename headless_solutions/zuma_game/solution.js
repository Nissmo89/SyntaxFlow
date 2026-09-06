var findMinStep = function(board, hand) {
    const memo = new Map();
    const clean = (s) => {
        for (let i = 0, j = 0; j <= s.length; j++) {
            if (j < s.length && s[j] === s[i]) continue;
            if (j - i >= 3) return clean(s.slice(0, i) + s.slice(j));
            i = j;
        }
        return s;
    };

    const solve = (b, h) => {
        if (!b) return 0;
        const key = b + "#" + h;
        if (memo.has(key)) return memo.get(key);
        let res = Infinity;
        for (let i = 0; i < h.length; i++) {
            for (let j = 0; j <= b.length; j++) {
                const nextB = clean(b.slice(0, j) + h[i] + b.slice(j));
                const nextH = h.slice(0, i) + h.slice(i + 1);
                const sub = solve(nextB, nextH);
                if (sub !== Infinity) res = Math.min(res, 1 + sub);
            }
        }
        memo.set(key, res === Infinity ? -1 : res);
        return memo.get(key);
    };
    return solve(board, hand);
};
