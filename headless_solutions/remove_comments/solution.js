/**
 * @param {string[]} source
 * @return {string[]}
 */
var removeComments = function(source) {
    let res = [], buffer = "", inBlock = false;
    for (let line of source) {
        for (let i = 0; i < line.length; i++) {
            if (inBlock) {
                if (line.slice(i, i + 2) === '*/') {
                    inBlock = false;
                    i++;
                }
            } else {
                if (line.slice(i, i + 2) === '/*') {
                    inBlock = true;
                    i++;
                } else if (line.slice(i, i + 2) === '//') {
                    break;
                } else {
                    buffer += line[i];
                }
            }
        }
        if (!inBlock && buffer.length > 0) {
            res.push(buffer);
            buffer = "";
        }
    }
    return res;
};
