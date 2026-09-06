var constructFromPrePost = function(preorder, postorder) {
    const postMap = new Map();
    for (let i = 0; i < postorder.length; i++) {
        postMap.set(postorder[i], i);
    }

    function build(preStart, preEnd, postStart, postEnd) {
        if (preStart > preEnd) return null;

        const root = new TreeNode(preorder[preStart]);
        if (preStart === preEnd) return root;

        // Left child value is the next element in preorder
        const leftRootVal = preorder[preStart + 1];
        const postIdx = postMap.get(leftRootVal);
        const leftSize = postIdx - postStart + 1;

        // Recursively construct left and right subtrees
        root.left = build(preStart + 1, preStart + leftSize, postStart, postIdx);
        root.right = build(preStart + leftSize + 1, preEnd, postIdx + 1, postEnd - 1);

        return root;
    }

    return build(0, preorder.length - 1, 0, postorder.length - 1);
};
