var largestValues = function(root) {
    if (!root) return [];
    const res = [], queue = [root];
    while (queue.length) {
        let size = queue.length, maxVal = -Infinity;
        for (let i = 0; i < size; i++) {
            const node = queue.shift();
            maxVal = Math.max(maxVal, node.val);
            if (node.left) queue.push(node.left);
            if (node.right) queue.push(node.right);
        }
        res.push(maxVal);
    }
    return res;
};
