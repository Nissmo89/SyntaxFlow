var kthSmallest = function(root, k) {
    let stack = [];
    let curr = root;
    while (curr || stack.length > 0) {
        while (curr) {
            stack.push(curr);
            curr = curr.left;
        }
        curr = stack.pop();
        if (--k === 0) return curr.val;
        curr = curr.right;
    }
};
