var movesToStamp = function(stamp, target) {
    let t = target.split(''), s = stamp.split(''), res = [];
    let visited = new Array(t.length - s.length + 1).fill(false);
    let stars = 0;
    while (stars < t.length) {
        let replaced = false;
        for (let i = 0; i <= t.length - s.length; i++) {
            if (!visited[i] && canReplace(t, i, s)) {
                stars += doReplace(t, i, s.length);
                visited[i] = true;
                res.push(i);
                replaced = true;
            }
        }
        if (!replaced) return [];
    }
    return res.reverse();
};
function canReplace(t, p, s) {
    for (let i = 0; i < s.length; i++) if (t[p + i] !== '?' && t[p + i] !== s[i]) return false;
    return true;
}
function doReplace(t, p, m) {
    let count = 0;
    for (let i = 0; i < m; i++) if (t[p + i] !== '?') { t[p + i] = '?'; count++; }
    return count;
}
