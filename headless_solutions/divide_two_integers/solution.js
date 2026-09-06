var divide = function(dividend, divisor) {
    if (dividend === -2147483648 && divisor === -1) return 2147483647;
    let dvd = Math.abs(dividend), dvs = Math.abs(divisor), ans = 0;
    let sign = (dividend < 0) ^ (divisor < 0) ? -1 : 1;
    while (dvd >= dvs) {
        let temp = dvs, m = 1;
        while ((temp << 1) <= dvd && (temp << 1) > 0) {
            temp <<= 1;
            m <<= 1;
        }
        dvd -= temp;
        ans += m;
    }
    return sign * ans;
};
