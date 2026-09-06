/**
 * @param {number} label
 * @return {number[]}
 */
var pathInZigZagTree = function(label) {
    let level = Math.floor(Math.log2(label)) + 1;
    let path = [];
    while (label >= 1) {
        path.push(label);
        label = Math.floor(label / 2);
        level--;
        label = (1 << (level - 1)) + (1 << level) - 1 - label;
    }
    return path.reverse();
};
