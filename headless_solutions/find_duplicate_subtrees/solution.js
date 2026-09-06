var findDuplicateSubtrees = function(root) {
    const serialToId = new Map();
    const idToCount = new Map();
    const res = [];
    let curId = 1;

    function postorder(node) {
        if (!node) return 0;
        const serial = `${postorder(node.left)},${node.val},${postorder(node.right)}`;
        if (!serialToId.has(serial)) serialToId.set(serial, curId++);
        const id = serialToId.get(serial);
        idToCount.set(id, (idToCount.get(id) || 0) + 1);
        if (idToCount.get(id) === 2) res.push(node);
        return id;
    }
    postorder(root);
    return res;
};
