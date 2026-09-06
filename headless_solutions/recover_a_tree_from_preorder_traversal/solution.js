var recoverFromPreorder = function(traversal) {
    let stack = [], i = 0;
    while (i < traversal.length) {
        let depth = 0;
        while (i < traversal.length && traversal[i] === '-') { depth++; i++; }
        let val = "";
        while (i < traversal.length && traversal[i] !== '-') { val += traversal[i++]; }
        let node = new TreeNode(parseInt(val));
        while (stack.length > depth) stack.pop();
        if (stack.length > 0) {
            if (!stack[stack.length - 1].left) stack[stack.length - 1].left = node;
            else stack[stack.length - 1].right = node;
        }
        stack.push(node);
    }
    return stack[0];
};
