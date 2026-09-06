/**
 * @param {number[]} arr
 * @return {number}
 */
var oddEvenJumps = function(arr) {
    const n = arr.length;
    const higher = new Array(n).fill(false);
    const lower = new Array(n).fill(false);
    higher[n - 1] = lower[n - 1] = true;
    
    // Using a simple map and sorting keys for simulation
    // Note: In production, use a balanced BST for O(log N)
    let map = new Map();
    map.set(arr[n - 1], n - 1);
    let sortedKeys = [arr[n - 1]];
    let res = 1;
    
    for (let i = n - 2; i >= 0; i--) {
        let val = arr[i];
        let hiIdx = sortedKeys.findIndex(k => k >= val);
        let loIdx = -1;
        for(let j = sortedKeys.length - 1; j >= 0; j--) {
            if(sortedKeys[j] <= val) { loIdx = j; break; }
        }
        
        if (hiIdx !== -1) higher[i] = lower[map.get(sortedKeys[hiIdx])];
        if (loIdx !== -1) lower[i] = higher[map.get(sortedKeys[loIdx])];
        
        if (higher[i]) res++;
        map.set(val, i);
        sortedKeys.splice(hiIdx === -1 ? sortedKeys.length : hiIdx, 0, val);
        sortedKeys.sort((a, b) => a - b);
    }
    return res;
};
