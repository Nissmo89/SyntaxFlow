var sumNumbers = function(root, currentSum = 0) {
    if (!root) return 0;
    currentSum = currentSum * 10 + root.val;
    if (!root.left && !root.right) return currentSum;
    return sumNumbers(root.left, currentSum) + sumNumbers(root.right, currentSum);
};
