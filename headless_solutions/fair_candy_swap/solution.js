/**
 * @param {number[]} aliceSizes
 * @param {number[]} bobSizes
 * @return {number[]}
 */
var fairCandySwap = function(aliceSizes, bobSizes) {
    let sumAlice = 0, sumBob = 0;
    for (let size of aliceSizes) sumAlice += size;
    for (let size of bobSizes) sumBob += size;
    
    let dif = (sumAlice - sumBob) / 2;
    let aliceSet = new Set(aliceSizes);
    
    for (let bobSize of bobSizes) {
        if (aliceSet.has(bobSize + dif)) {
            return [bobSize + dif, bobSize];
        }
    }
    
    // This should not happen given the problem constraints
    return [];
};
