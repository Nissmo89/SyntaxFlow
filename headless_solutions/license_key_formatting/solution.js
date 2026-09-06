/**
 * @param {string} s
 * @param {number} k
 * @return {string}
 */
var licenseKeyFormatting = function(s, k) {
    s = s.replace("-", "").toUpperCase().split("").reverse().join("");
    let ans = "";
    let count = 0;
    for (let char of s) {
        if (count == k) {
            ans += "-";
        }
        count = (count + 1) % k;
        ans += char;
    }
    return ans.split("").reverse().join("");
};
