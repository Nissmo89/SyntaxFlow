var minDominoRotations = function(tops, bottoms) {
    const check = (target) => {
        let topRot = 0, botRot = 0;
        for (let i = 0; i < tops.length; i++) {
            if (tops[i] !== target && bottoms[i] !== target) return Infinity;
            if (tops[i] !== target) topRot++;
            if (bottoms[i] !== target) botRot++;
        }
        return Math.min(topRot, botRot);
    };
    const res = Math.min(check(tops[0]), check(bottoms[0]));
    return res === Infinity ? -1 : res;
};
