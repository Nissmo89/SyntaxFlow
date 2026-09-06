var reverse = function(n) {
    let sign = (n < 0) ? -1 : 1;
    n = Math.abs(n);
    let reversed = 0;
    while (n > 0) {
        reversed = reversed * 10 + n % 10;
        n = Math.floor(n / 10);
    }
    reversed *= sign;
    // Check for overflow
    if (reversed < -2**31 || reversed > 2**31 - 1) {
        return 0;
    }
    return reversed;
};
