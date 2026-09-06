class Solution {
private:
    // Helper function to calculate the length of the singly linked list
    int getLength(ListNode* head) {
        int length = 0;
        while (head != nullptr) {
            length++;
            head = head->next;
        }
        return length;
    }

    // Helper function to build BST in-order
    TreeNode* buildBST(ListNode*& curr, int start, int end) {
        if (start > end) {
            return nullptr;
        }

        int mid = start + (end - start) / 2;

        // 1. Recursively construct the left subtree
        TreeNode* leftChild = buildBST(curr, start, mid - 1);

        // 2. Process current root node
        TreeNode* root = new TreeNode(curr->val);
        root->left = leftChild;

        // Advance linked list pointer to the next element
        curr = curr->next;

        // 3. Recursively construct the right subtree
        root->right = buildBST(curr, mid + 1, end);

        return root;
    }

public:
    TreeNode* sortedListToBST(ListNode* head) {
        int length = getLength(head);
        return buildBST(head, 0, length - 1);
    }
};
