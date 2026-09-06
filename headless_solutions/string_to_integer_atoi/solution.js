var myAtoi = function(s) {
    let i = 0, sign = 1, res = 0;
    s = s.trimStart();
    if (s[i] === '-' || s[i] === '+') {
        sign = s[i++] === '-' ? -1 : 1;
    }
    while (i < s.length && s[i] >= '0' && s[i] <= '9') {
        res = res * 10 + Number(s[i++]);
        if (res * sign > 2147483647) return 2147483647;
        if (res * sign < -2147483648) return -2147483648;
    }
    return res * sign;
};
