var letterCombinations = function(digits) {
    if (!digits) return [];
    const mapping = ["", "", "abc", "def", "ghi", "jkl", "mno", "pqrs", "tuv", "wxyz"];
    const res = [];
    
    function backtrack(idx, path) {
        if (idx === digits.length) {
            res.push(path);
            return;
        }
        for (const char of mapping[digits[idx]]) {
            backtrack(idx + 1, path + char);
        }
    }
    backtrack(0, "");
    return res;
};
