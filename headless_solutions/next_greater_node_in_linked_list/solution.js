var nextLargerNodes = function(head) {
    let vals = [];
    while (head) {
        vals.push(head.val);
        head = head.next;
    }
    let res = new Array(vals.length).fill(0);
    let stack = [];
    for (let i = 0; i < vals.length; i++) {
        while (stack.length > 0 && vals[stack[stack.length - 1]] < vals[i]) {
            res[stack.pop()] = vals[i];
        }
        stack.push(i);
    }
    return res;
};
