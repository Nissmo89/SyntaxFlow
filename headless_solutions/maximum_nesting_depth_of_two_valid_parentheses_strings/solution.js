/**
 * @param {string} seq
 * @return {number[]}
 */
var maxDepthAfterSplit = function(seq) {
    let res = new Array(seq.length);
    let depth = 0;
    for (let i = 0; i < seq.length; i++) {
        if (seq[i] === '(') {
            res[i] = depth++ % 2;
        } else {
            res[i] = --depth % 2;
        }
    }
    return res;
};
