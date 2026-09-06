var decodeString = function(s) {
    let stack = [];
    let curNum = 0;
    let curStr = "";
    
    for (let char of s) {
        if (!isNaN(char)) {
            curNum = curNum * 10 + parseInt(char);
        } else if (char === '[') {
            stack.push([curStr, curNum]);
            curStr = "";
            curNum = 0;
        } else if (char === ']') {
            let [prevStr, num] = stack.pop();
            curStr = prevStr + curStr.repeat(num);
        } else {
            curStr += char;
        }
    }
    return curStr;
};
