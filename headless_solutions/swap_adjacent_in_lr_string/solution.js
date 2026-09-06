/**
 * @param {string} start
 * @param {string} result
 * @return {boolean}
 */
var canTransform = function(start, result) {
    let i = 0, j = 0;
    const n = start.length;
    while (i < n || j < n) {
        while (i < n && start[i] === 'X') i++;
        while (j < n && result[j] === 'X') j++;
        
        if (i === n || j === n) return i === j;
        if (start[i] !== result[j]) return false;
        if (start[i] === 'L' && i < j) return false;
        if (start[i] === 'R' && i > j) return false;
        i++; j++;
    }
    return true;
};
