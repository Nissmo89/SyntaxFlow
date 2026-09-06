class Solution {
public:
    int getMinimumDifference(TreeNode* root) {
        int min_diff = INT_MAX;
        int prev_val = -1;
        
        inorderTraverse(root, min_diff, prev_val);
        
        return min_diff;
    }
    
    void inorderTraverse(TreeNode* root, int& min_diff, int& prev_val) {
        if (root->left != NULL) {
            inorderTraverse(root->left, min_diff, prev_val);
        }
        
        if (prev_val >= 0) {
            min_diff = min(min_diff, root->val - prev_val);
        }
        
        prev_val = root->val;
        
        if (root->right != NULL) {
            inorderTraverse(root->right, min_diff, prev_val);
        }
    }
};
