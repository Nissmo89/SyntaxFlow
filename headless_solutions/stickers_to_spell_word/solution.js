var minStickers = function(stickers, target) {
    const memo = new Map([["", 0]]);
    const counts = stickers.map(s => {
        const c = new Array(26).fill(0);
        for (let char of s) c[char.charCodeAt(0) - 97]++;
        return c;
    });

    function solve(t) {
        if (memo.has(t)) return memo.get(t);
        const tar = new Array(26).fill(0);
        for (let char of t) tar[char.charCodeAt(0) - 97]++;
        
        let res = Infinity;
        const firstCharIdx = t.charCodeAt(0) - 97;
        for (let i = 0; i < counts.length; i++) {
            if (counts[i][firstCharIdx] === 0) continue;
            let nextT = "";
            for (let j = 0; j < 26; j++) {
                if (tar[j] > counts[i][j]) {
                    nextT += String.fromCharCode(97 + j).repeat(tar[j] - counts[i][j]);
                }
            }
            res = Math.min(res, 1 + solve(nextT));
        }
        memo.set(t, res);
        return res;
    }
    const ans = solve(target);
    return ans === Infinity ? -1 : ans;
};
