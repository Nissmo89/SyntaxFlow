var fractionToDecimal = function(numerator, denominator) {
    if (numerator === 0) return "0";
    let res = "";
    if ((numerator < 0) ^ (denominator < 0)) res += "-";
    
    let n = Math.abs(numerator);
    let d = Math.abs(denominator);
    
    res += Math.floor(n / d);
    let rem = n % d;
    if (rem === 0) return res;
    
    res += ".";
    let map = new Map();
    while (rem !== 0) {
        if (map.has(rem)) {
            res = res.slice(0, map.get(rem)) + "(" + res.slice(map.get(rem)) + ")";
            break;
        }
        map.set(rem, res.length);
        rem *= 10;
        res += Math.floor(rem / d);
        rem %= d;
    }
    return res;
};
