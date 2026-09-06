class Solution {
public:
    TreeNode* constructMaximumBinaryTree(vector<int>& nums) {
        vector<TreeNode*> stk;
        for (int val : nums) {
            TreeNode* cur = new TreeNode(val);
            while (!stk.empty() && stk.back()->val < val) {
                cur->left = stk.back();
                stk.pop_back();
            }
            if (!stk.empty()) {
                stk.back()->right = cur;
            }
            stk.push_back(cur);
        }
        return stk.front();
    }
};
