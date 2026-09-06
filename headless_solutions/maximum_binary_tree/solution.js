var constructMaximumBinaryTree = function(nums) {
    let stack = [];
    for (let val of nums) {
        let cur = new TreeNode(val);
        while (stack.length > 0 && stack[stack.length - 1].val < val) {
            cur.left = stack.pop();
        }
        if (stack.length > 0) {
            stack[stack.length - 1].right = cur;
        }
        stack.push(cur);
    }
    return stack[0];
};
