/**
 * @param {number} a
 * @param {number[]} b
 * @return {number}
 */
var superPow = function(a, b) {
    const MOD = 1337;
    const myPow = (base, exp) => {
        let res = 1;
        base %= MOD;
        for (let i = 0; i < exp; i++) res = (res * base) % MOD;
        return res;
    };
    let res = 1;
    for (let d of b) {
        res = (myPow(res, 10) * myPow(a, d)) % MOD;
    }
    return res;
};
