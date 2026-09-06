/**
 * @param {number} n
 * @return {number}
 */
var bitwiseComplement = function(n) {
    if (n == 0) return 1;
    let X = 1;
    while (n > X) X = (X << 1) + 1;
    return X ^ n;
};
