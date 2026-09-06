class Solution {
public:
    bool isSameTree(TreeNode* p, TreeNode* q) {
        // Base case: if both trees are null, they are identical
        if (p == nullptr && q == nullptr) {
            return true;
        }
        // If only one tree is null or the values are different, they are not identical
        if (p == nullptr || q == nullptr || p->val != q->val) {
            return false;
        }
        // Recursively check if the left and right subtrees are identical
        return isSameTree(p->left, q->left) && isSameTree(p->right, q->right);
    }
};
