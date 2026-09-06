class Solution {
public:
    int findTilt(TreeNode* root) {
        int result = 0;
        postOrder(root, result);
        return result;
    }
    
    int postOrder(TreeNode* node, int& result) {
        if (node == nullptr) return 0;
        
        int left = postOrder(node->left, result);
        int right = postOrder(node->right, result);
        
        result += abs(left - right);
        
        return left + right + node->val;
    }
};
