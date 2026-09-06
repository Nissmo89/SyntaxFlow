var generateTrees = function(n) {
    const memo = new Map();
    const generate = (start, end) => {
        if (start > end) return [null];
        const key = `${start}-${end}`;
        if (memo.has(key)) return memo.get(key);
        
        const res = [];
        for (let i = start; i <= end; i++) {
            const lefts = generate(start, i - 1);
            const rights = generate(i + 1, end);
            for (let l of lefts) {
                for (let r of rights) {
                    res.push(new TreeNode(i, l, r));
                }
            }
        }
        memo.set(key, res);
        return res;
    };
    return generate(1, n);
};
