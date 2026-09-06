/**
 * @param {number[]} hand
 * @param {number} groupSize
 * @return {boolean}
 */
var isNStraightHand = function(hand, groupSize) {
    // Quick reject
    if (hand.length % groupSize !== 0) return false;

    // Build frequency map
    const cnt = new Map();
    for (const v of hand) {
        cnt.set(v, (cnt.get(v) || 0) + 1);
    }

    // Get sorted unique keys
    const keys = Array.from(cnt.keys()).sort((a, b) => a - b);

    for (const start of keys) {
        const need = cnt.get(start);
        if (need === 0) continue;

        // Consume a consecutive block
        for (let i = 0; i < groupSize; ++i) {
            const key = start + i;
            const cur = cnt.get(key) ?? 0;
            if (cur < need) return false;
            cnt.set(key, cur - need);
        }
    }
    return true;
};
