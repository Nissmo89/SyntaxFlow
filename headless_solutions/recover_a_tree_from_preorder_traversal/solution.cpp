class Solution {
public:
    TreeNode* recoverFromPreorder(string traversal) {
        vector<TreeNode*> stack;
        int i = 0, n = traversal.length();
        while (i < n) {
            int depth = 0;
            while (i < n && traversal[i] == '-') { depth++; i++; }
            int val = 0;
            while (i < n && isdigit(traversal[i])) { val = val * 10 + (traversal[i++] - '0'); }
            
            TreeNode* node = new TreeNode(val);
            while (stack.size() > depth) stack.pop_back();
            
            if (!stack.empty()) {
                if (!stack.back()->left) stack.back()->left = node;
                else stack.back()->right = node;
            }
            stack.push_back(node);
        }
        return stack[0];
    }
};
