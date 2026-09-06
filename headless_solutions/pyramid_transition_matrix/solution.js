/**
 * @param {string} bottom
 * @param {string[]} allowed
 * @return {boolean}
 */
var pyramidTransition = function(bottom, allowed) {
    const map = new Map();
    for (const s of allowed) {
        const key = s.substring(0, 2);
        if (!map.has(key)) map.set(key, []);
        map.get(key).push(s[2]);
    }

    function solve(curr, next, idx) {
        if (curr.length === 1) return true;
        if (idx === curr.length - 1) return solve(next, "", 0);
        
        const pair = curr.substring(idx, idx + 2);
        if (!map.has(pair)) return false;
        
        for (const char of map.get(pair)) {
            if (solve(curr, next + char, idx + 1)) return true;
        }
        return false;
    }
    
    return solve(bottom, "", 0);
};
