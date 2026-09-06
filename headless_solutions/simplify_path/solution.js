/**
 * @param {string} path
 * @return {string}
 */
var simplifyPath = function(path) {
    const stack = [];
    const tokens = path.split('/');
    
    for (const token of tokens) {
        if (token === "" || token === ".") continue;
        if (token === "..") {
            if (stack.length > 0) stack.pop();
        } else {
            stack.push(token);
        }
    }
    
    return "/" + stack.join("/");
};
