var widthOfBinaryTree = function(root) {
    if (!root) return 0;
    let maxWidth = 0;
    let queue = [[root, 0n]];
    
    while (queue.length > 0) {
        let size = queue.length;
        let start = queue[0][1];
        let end = queue[size - 1][1];
        maxWidth = Math.max(maxWidth, Number(end - start + 1n));
        
        for (let i = 0; i < size; i++) {
            let [node, idx] = queue.shift();
            if (node.left) queue.push([node.left, 2n * (idx - start)]);
            if (node.right) queue.push([node.right, 2n * (idx - start) + 1n]);
        }
    }
    return maxWidth;
};
