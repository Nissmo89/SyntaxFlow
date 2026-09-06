/**
 * @param {number} a, b, c
 * @return {number[]}
 */
var numMovesStones = function(a, b, c) {
    let s = [a, b, c].sort((x, y) => x - y);
    
    if (s[2] - s[0] === 2) return [0, 0];
    
    let minMoves = (s[1] - s[0] <= 2 || s[2] - s[1] <= 2) ? 1 : 2;
    let maxMoves = s[2] - s[0] - 2;
    
    return [minMoves, maxMoves];
};
