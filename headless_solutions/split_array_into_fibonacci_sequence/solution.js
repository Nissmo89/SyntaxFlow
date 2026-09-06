/**
 * @param {string} num
 * @return {number[]}
 */
var splitIntoFibonacci = function(num) {
    const res = [];
    const backtrack = (idx) => {
        if (idx === num.length && res.length >= 3) return true;
        for (let i = idx; i < num.length; i++) {
            if (num[idx] === '0' && i > idx) break;
            const val = Number(num.substring(idx, i + 1));
            if (val > 2147483647) break;
            
            const sz = res.length;
            if (sz >= 2 && val > res[sz - 1] + res[sz - 2]) break;
            
            if (sz < 2 || val === res[sz - 1] + res[sz - 2]) {
                res.push(val);
                if (backtrack(i + 1)) return true;
                res.pop();
            }
        }
        return false;
    };
    backtrack(0);
    return res;
};
