var levelOrderBottom = function(root) {
    if (!root) return [];

    let levels = [];
    let queue = [root];

    while (queue.length) {
        let levelSize = queue.length;
        let level = [];

        for (let i = 0; i < levelSize; i++) {
            let node = queue.shift();
            level.push(node.val);

            if (node.left) queue.push(node.left);
            if (node.right) queue.push(node.right);
        }

        levels.unshift(level);
    }

    return levels;
};
