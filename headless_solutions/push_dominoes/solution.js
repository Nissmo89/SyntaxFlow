var pushDominoes = function(dominoes) {
    let s = 'L' + dominoes + 'R';
    let res = [];
    for (let i = 0, j = 1; j < s.length; j++) {
        if (s[j] === '.') continue;
        let mid = j - i - 1;
        if (i > 0) res.push(s[i]);
        if (s[i] === s[j]) res.push(s[i].repeat(mid));
        else if (s[i] === 'L' && s[j] === 'R') res.push('.'.repeat(mid));
        else res.push('R'.repeat(mid >> 1) + (mid % 2 ? '.' : '') + 'L'.repeat(mid >> 1));
        i = j;
    }
    return res.join('');
};
