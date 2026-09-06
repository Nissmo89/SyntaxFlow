var isNumber = function(s) {
    let seenDigit = false, seenDot = false, seenE = false, numberAfterE = true;
    for (let i = 0; i < s.length; i++) {
        let c = s[i];
        if (c >= '0' && c <= '9') {
            seenDigit = true;
            numberAfterE = true;
        } else if (c === '.') {
            if (seenDot || seenE) return false;
            seenDot = true;
        } else if (c === 'e' || c === 'E') {
            if (seenE || !seenDigit) return false;
            seenE = true;
            numberAfterE = false;
        } else if (c === '+' || c === '-') {
            if (i > 0 && s[i - 1] !== 'e' && s[i - 1] !== 'E') return false;
        } else {
            return false;
        }
    }
    return seenDigit && numberAfterE;
};
