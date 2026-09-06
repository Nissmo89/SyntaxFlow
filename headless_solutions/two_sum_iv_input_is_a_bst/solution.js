var findTarget = function(root, k) {
    let set = new Set();
    return dfs(root, set, k);

    function dfs(root, set, k) {
        if (!root) return false;
        if (set.has(k - root.val)) return true;
        set.add(root.val);
        return dfs(root.left, set, k) || dfs(root.right, set, k);
    }
};
