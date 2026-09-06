/**
 * @param {string} s
 * @param {string} goal
 * @return {boolean}
 */
var buddyStrings = function(s, goal) {
    if (s.length !== goal.length) return false;
    if (s === goal) {
        let unique_chars = new Set(s);
        return unique_chars.size < s.length;
    }
    let dif = [];
    for (let i = 0; i < s.length; ++i) if (s[i] !== goal[i]) dif.push(i);
    return dif.length === 2 && s[dif[0]] === goal[dif[1]] && s[dif[1]] === goal[dif[0]];
};
