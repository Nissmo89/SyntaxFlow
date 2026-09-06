var rightSideView = function(root) {
    let result = [];
    rightView(root, result, 0);
    return result;

    function rightView(curr, result, currDepth) {
        if (curr == null) return;
        if (currDepth == result.length) result.push(curr.val);

        rightView(curr.right, result, currDepth + 1);
        rightView(curr.left, result, currDepth + 1);
    }
};
