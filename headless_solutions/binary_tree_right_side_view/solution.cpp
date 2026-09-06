class Solution {
public:
    vector<int> rightSideView(TreeNode* root) {
        vector<int> result;
        rightView(root, result, 0);
        return result;
    }

    void rightView(TreeNode* curr, vector<int>& result, int currDepth) {
        if (curr == nullptr) return;
        if (currDepth == result.size()) result.push_back(curr->val);

        rightView(curr->right, result, currDepth + 1);
        rightView(curr->left, result, currDepth + 1);
    }
};
