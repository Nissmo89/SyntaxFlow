/**
 * @param {number[][]} stones
 * @return {number}
 */
var removeStones = function(stones) {
    const parent = new Map();
    let components = 0;

    const find = (i) => {
        if (!parent.has(i)) {
            parent.set(i, i);
            components++;
        }
        if (parent.get(i) === i) return i;
        parent.set(i, find(parent.get(i)));
        return parent.get(i);
    };

    for (const [r, c] of stones) {
        const rootR = find(r);
        const rootC = find(c + 10001);
        if (rootR !== rootC) {
            parent.set(rootR, rootC);
            components--;
        }
    }
    return stones.length - components;
};
