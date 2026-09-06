/**
 * @param {number[]} fronts
 * @param {number[]} backs
 * @return {number}
 */
var flipgame = function(fronts, backs) {
    const fixedNumbers = new Set();
    const n = fronts.length;

    // Step 1: Identify fixed numbers
    for (let i = 0; i < n; i++) {
        if (fronts[i] === backs[i]) {
            fixedNumbers.add(fronts[i]);
        }
    }

    let minGood = Infinity;
    let found = false;

    // Step 2: Check all numbers in fronts and backs
    const allNumbers = [...fronts,...backs];
    for (const val of allNumbers) {
        if (!fixedNumbers.has(val)) {
            if (val < minGood) {
                minGood = val;
                found = true;
            }
        }
    }

    return found? minGood : 0;
};
