var findLUSlength = function(strs) {
    const isSubsequence = (s1, s2) => {
        let i = 0, j = 0;
        while (i < s1.length && j < s2.length) {
            if (s1[i] === s2[j]) i++;
            j++;
        }
        return i === s1.length;
    };

    strs.sort((a, b) => b.length - a.length);

    for (let i = 0; i < strs.length; i++) {
        let isCommon = false;
        for (let j = 0; j < strs.length; j++) {
            if (i === j) continue;
            if (isSubsequence(strs[i], strs[j])) {
                isCommon = true;
                break;
            }
        }
        if (!isCommon) return strs[i].length;
    }
    return -1;
};
