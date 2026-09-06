var delNodes = function(root, to_delete) {
    const toDeleteSet = new Set(to_delete);
    const forest = [];

    const helper = (node, isRoot) => {
        if (!node) return null;
        const deleted = toDeleteSet.has(node.val);
        if (isRoot && !deleted) forest.push(node);

        node.left = helper(node.left, deleted);
        node.right = helper(node.right, deleted);

        return deleted ? null : node;
    };

    helper(root, true);
    return forest;
};
