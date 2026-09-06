/**
 * @param {TreeNode} root
 * @return {number[]}
 */
var findMode = function(root) {
    let counter = {};
    dfs(root, counter);
    let maxFreq = 0;
    for (let freq in counter) {
        maxFreq = Math.max(maxFreq, parseInt(freq));
    }
    let modes = [];
    for (let val in counter) {
        if (counter[val] == maxFreq) {
            modes.push(parseInt(val));
        }
    }
    return modes;
};

function dfs(node, counter) {
    if (node == null) return;
    counter[node.val] = (counter[node.val] || 0) + 1;
    dfs(node.left, counter);
    dfs(node.right, counter);
}
