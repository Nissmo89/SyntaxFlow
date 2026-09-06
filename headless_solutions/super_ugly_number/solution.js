var nthSuperUglyNumber = function(n, primes) {
    let ugly = new Int32Array(n);
    ugly[0] = 1;
    let k = primes.length;
    let idx = new Int32Array(k);
    let val = new Int32Array(k).fill(1);
    let next = 1;

    for (let i = 0; i < n; i++) {
        ugly[i] = next;
        next = Infinity;
        for (let j = 0; j < k; j++) {
            if (val[j] === ugly[i]) val[j] = primes[j] * ugly[idx[j]++];
            if (val[j] < next) next = val[j];
        }
    }
    return ugly[n - 1];
};
