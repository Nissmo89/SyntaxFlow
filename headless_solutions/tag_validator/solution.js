/**
 * @param {string} code
 * @return {boolean}
 */
var isValid = function(code) {
    let stack = [];
    for (let i = 0; i < code.length; ) {
        if (i > 0 && stack.length === 0) return false;
        if (code.startsWith("<![CDATA[", i)) {
            let j = code.indexOf("]]>", i + 9);
            if (j === -1) return false;
            i = j + 3;
        } else if (code.startsWith("</", i)) {
            let j = code.indexOf('>', i + 2);
            if (j === -1) return false;
            let tag = code.substring(i + 2, j);
            if (stack.pop() !== tag) return false;
            i = j + 1;
        } else if (code.startsWith("<", i)) {
            let j = code.indexOf('>', i + 1);
            if (j === -1 || j - i - 1 < 1 || j - i - 1 > 9) return false;
            let tag = code.substring(i + 1, j);
            if (!/^[A-Z]+$/.test(tag)) return false;
            stack.push(tag);
            i = j + 1;
        } else {
            i++;
        }
    }
    return stack.length === 0;
};
