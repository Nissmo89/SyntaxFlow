/**
 * @param {number[][]} ghosts
 * @param {number[]} target
 * @return {boolean}
 */
var escapeGhosts = function(ghosts, target) {
    const playerDist = Math.abs(target[0]) + Math.abs(target[1]);
    for (const [gx, gy] of ghosts) {
        const ghostDist = Math.abs(gx - target[0]) + Math.abs(gy - target[1]);
        if (ghostDist <= playerDist) return false;
    }
    return true;
};
