var wordBreak = function(s, wordDict) {
    const dict = new Set(wordDict);
    const memo = new Map();

    const dfs = (target) => {
        if (memo.has(target)) return memo.get(target);
        if (target === "") return [""];
        
        const res = [];
        for (const word of dict) {
            if (target.startsWith(word)) {
                const sub = dfs(target.slice(word.length));
                for (const str of sub) {
                    res.push(word + (str === "" ? "" : " ") + str);
                }
            }
        }
        memo.set(target, res);
        return res;
    };
    return dfs(s);
};
