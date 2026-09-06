/**
 * @param {string} str1
 * @param {string} str2
 * @return {string}
 */
var gcdOfStrings = function(str1, str2) {
    if (str1.length < str2.length) {
        return gcdOfStrings(str2, str1);
    }
    if (str1 + str2 !== str2 + str1) {
        return "";
    }
    var base_len = gcd(str1.length, str2.length);
    return str1.substring(0, base_len);
};
