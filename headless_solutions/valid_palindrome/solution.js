/**
 * @param {string} s
 * @return {boolean}
 */
var isPalindrome = function(s) {
    let start = 0;
    let end = s.length - 1;
    while (start <= end) {
        if (!/^[a-zA-Z0-9]$/.test(s[start])) {
            start++;
            continue;
        }
        if (!/^[a-zA-Z0-9]$/.test(s[end])) {
            end--;
            continue;
        }
        if (s[start].toLowerCase() !== s[end].toLowerCase()) {
            return false;
        } else {
            start++;
            end--;
        }
    }
    return true;
};
