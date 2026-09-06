var nthMagicalNumber = function(n, a, b) {
    const gcd = (x, y) => (y === 0 ? x : gcd(y, x % y));
    const lcm = (BigInt(a) * BigInt(b)) / BigInt(gcd(a, b));
    let left = 2n, right = BigInt(n) * BigInt(Math.min(a, b));
    const mod = 1000000007n;
    
    while (left < right) {
        let mid = left + (right - left) / 2n;
        if (mid / BigInt(a) + mid / BigInt(b) - mid / lcm < BigInt(n)) {
            left = mid + 1n;
        } else {
            right = mid;
        }
    }
    return Number(left % mod);
};
