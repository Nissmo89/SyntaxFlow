/**
 * @param {ListNode} head
 * @return {TreeNode}
 */
var sortedListToBST = function(head) {
    function getLength(node) {
        let length = 0;
        while (node !== null) {
            length++;
            node = node.next;
        }
        return length;
    }

    const length = getLength(head);
    let curr = head;

    function buildBST(start, end) {
        if (start > end) {
            return null;
        }

        const mid = Math.floor(start + (end - start) / 2);

        // 1. Construct left child
        const leftChild = buildBST(start, mid - 1);

        // 2. Process root node
        const root = new TreeNode(curr.val);
        root.left = leftChild;

        // Advance linked list reference
        curr = curr.next;

        // 3. Construct right child
        root.right = buildBST(mid + 1, end);

        return root;
    }

    return buildBST(0, length - 1);
};
