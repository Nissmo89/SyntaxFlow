class Solution {
public:
    int widthOfBinaryTree(TreeNode* root) {
        if (!root) return 0;
        unsigned long long maxWidth = 0;
        queue<pair<TreeNode*, unsigned long long>> q;
        q.push({root, 0});
        
        while (!q.empty()) {
            int size = q.size();
            unsigned long long start = q.front().second;
            unsigned long long end = start;
            
            for (int i = 0; i < size; ++i) {
                auto [node, idx] = q.front(); q.pop();
                end = idx;
                if (node->left) q.push({node->left, 2 * (idx - start)});
                if (node->right) q.push({node->right, 2 * (idx - start) + 1});
            }
            maxWidth = max(maxWidth, end - 0 + 1);
        }
        return (int)maxWidth;
    }
};
