/**
 * @param {string} num
 * @return {boolean}
 */
var isAdditiveNumber = function(num) {
    for (let i = 1; i <= num.length / 2; i++) {
        for (let j = 1; j <= (num.length - i) / 2; j++) {
            if (check(num.substring(0, i), num.substring(i, i + j), num.substring(i + j))) {
                return true;
            }
        }
    }
    return false;
};

function check(num1, num2, num) {
    if (num1.length > 1 && num1[0] == '0' || num2.length > 1 && num2[0] == '0') {
        return false;
    }
    let sum = add(num1, num2);
    if (num == sum) {
        return true;
    }
    if (num.length <= sum.length || num.substring(0, sum.length) == sum) {
        return false;
    } else {
        return check(num2, sum, num.substring(sum.length));
    }
}

function add(n, m) {
    let res = '';
    let i = n.length - 1, j = m.length - 1, carry = 0;
    while (i >= 0 || j >= 0) {
        let sum = carry + (i >= 0 ? n.charCodeAt(i--) - '0'.charCodeAt(0) : 0) + (j >= 0 ? m.charCodeAt(j--) - '0'.charCodeAt(0) : 0);
        res = (sum % 10).toString() + res;
        carry = Math.floor(sum / 10);
    }
    if (carry > 0) {
        res = carry.toString() + res;
    }
    return res.split('').reverse().join('');
}
