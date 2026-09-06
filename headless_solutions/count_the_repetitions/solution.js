// JavaScript implementation matching signature
/**
 * @param {string} s1
 * @param {number} n1
 * @param {string} s2
 * @param {number} n2
 * @return {number}
 */
var getMaxRepetitions = function(s1, n1, s2, n2) {
    const record = new Map(); // pos in s2 -> [count1, count2]
    let count1 = 0, count2 = 0;
    let i = 0, j = 0;

    while (count1 < n1) {
        if (s1[i] === s2[j]) {
            j++;
            if (j === s2.length) {
                j = 0;
                count2++;
            }
        }
        i++;
        if (i === s1.length) {
            i = 0;
            count1++;
        }

        if (record.has(j)) {
            const [prevCount1, prevCount2] = record.get(j);
            const preLoopCount1 = prevCount1;
            const preLoopCount2 = prevCount2;

            const loopLen = count1 - prevCount1;
            const loopCnt = count2 - prevCount2;

            const remaining = n1 - preLoopCount1;
            const fullCycles = Math.floor(remaining / loopLen);

            const totalCount2 = preLoopCount2
                + fullCycles * loopCnt
                + (remaining % loopLen) * Math.floor(loopCnt / loopLen);

            return Math.floor(totalCount2 / n2);
        } else {
            record.set(j, [count1, count2]);
        }
    }
    return Math.floor(count2 / n2);
};
