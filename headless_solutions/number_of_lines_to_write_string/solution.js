/**
 * @param {number[]} widths
 * @param {string} s
 * @return {number[]}
 */
var numberOfLines = function(widths, s) {
    let lines = 1, width = 0;
    for (let c of s) {
        let w = widths[c.charCodeAt(0) - 'a'.charCodeAt(0)];
        if (width + w > 100) {
            lines++;
            width = w;
        } else {
            width += w;
        }
    }
    return [lines, width];
};
