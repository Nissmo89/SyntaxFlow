/**
 * @param {number} sx, @param {number} sy, @param {number} tx, @param {number} ty
 * @return {boolean}
 */
var reachingPoints = function(sx, sy, tx, ty) {
    while (tx >= sx && ty >= sy) {
        if (tx === sx && ty === sy) return true;
        if (tx > ty) {
            if (ty === sy) return (tx - sx) % ty === 0;
            tx %= ty;
        } else {
            if (tx === sx) return (ty - sy) % tx === 0;
            ty %= tx;
        }
    }
    return false;
};
