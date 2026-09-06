#include <string>

class Solution {
private:
    void dfs(TreeNode* node, std::string& result) {
        if (!node) return;

        // Append root value
        result += std::to_string(node->val);

        // Process left child if left exists OR right exists (to keep '()')
        if (node->left || node->right) {
            result += "(";
            dfs(node->left, result);
            result += ")";
        }

        // Process right child only if right exists
        if (node->right) {
            result += "(";
            dfs(node->right, result);
            result += ")";
        }
    }

public:
    std::string tree2str(TreeNode* root) {
        std::string result = "";
        dfs(root, result);
        return result;
    }
};
