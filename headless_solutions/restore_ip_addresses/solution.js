var restoreIpAddresses = function(s) {
    const res = [];
    const backtrack = (start, path) => {
        if (path.length === 4) {
            if (start === s.length) res.push(path.join('.'));
            return;
        }
        for (let len = 1; len <= 3 && start + len <= s.length; len++) {
            const sub = s.substring(start, start + len);
            if ((sub.length > 1 && sub[0] === '0') || parseInt(sub) > 255) continue;
            backtrack(start + len, [...path, sub]);
        }
    };
    backtrack(0, []);
    return res;
};
