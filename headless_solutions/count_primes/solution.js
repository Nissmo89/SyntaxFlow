/**
 * @param {number} n
 * @return {number}
 */
var countPrimes = function(n) {
    if (n <= 2) return 0;
    
    // Uint8Array is memory efficient and cache friendly
    const isComposite = new Uint8Array(n);
    
    for (let i = 2; i * i < n; i++) {
        if (!isComposite[i]) {
            for (let j = i * i; j < n; j += i) {
                isComposite[j] = 1;
            }
        }
    }
    
    let count = 0;
    for (let i = 2; i < n; i++) {
        if (!isComposite[i]) {
            count++;
        }
    }
    
    return count;
};
