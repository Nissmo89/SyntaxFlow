class Solution {
public:
    int findBottomLeftValue(TreeNode* root) {
        queue<TreeNode*> q;
        q.push(root);
        TreeNode* curr = nullptr;
        while (!q.empty()) {
            curr = q.front();
            q.pop();
            // Push right then left to ensure the leftmost node is processed last
            if (curr->right) q.push(curr->right);
            if (curr->left) q.push(curr->left);
        }
        return curr->val;
    }
};
