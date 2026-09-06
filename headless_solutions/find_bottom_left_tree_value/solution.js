var findBottomLeftValue = function(root) {
    let queue = [root];
    let curr = null;
    while (queue.length > 0) {
        curr = queue.shift();
        if (curr.right) queue.push(curr.right);
        if (curr.left) queue.push(curr.left);
    }
    return curr.val;
};
