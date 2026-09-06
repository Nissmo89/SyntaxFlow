/**
 * @param {number} n
 * @return {number}
 */
var monotoneIncreasingDigits = function(n) {
    let s = n.toString().split('');
    let marker = s.length;
    for (let i = s.length - 1; i > 0; i--) {
        if (s[i - 1] > s[i]) {
            s[i - 1] = (parseInt(s[i - 1]) - 1).toString();
            marker = i;
        }
    }
    for (let i = marker; i < s.length; i++) {
        s[i] = '9';
    }
    return parseInt(s.join(''));
};
