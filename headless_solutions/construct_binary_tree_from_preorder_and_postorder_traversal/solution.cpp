#include <vector>
#include <unordered_map>

using namespace std;

class Solution {
private:
    unordered_map<int, int> post_map;

    TreeNode* build(const vector<int>& preorder, int preStart, int preEnd,
                   const vector<int>& postorder, int postStart, int postEnd) {
        if (preStart > preEnd) return nullptr;
        
        TreeNode* root = new TreeNode(preorder[preStart]);
        if (preStart == preEnd) return root;

        // Left child value is the next element in preorder
        int leftRootVal = preorder[preStart + 1];
        int postIdx = post_map[leftRootVal];
        int leftSize = postIdx - postStart + 1;

        // Recursively construct left and right subtrees
        root->left = build(preorder, preStart + 1, preStart + leftSize,
                           postorder, postStart, postIdx);
        root->right = build(preorder, preStart + leftSize + 1, preEnd,
                            postorder, postIdx + 1, postEnd - 1);

        return root;
    }

public:
    TreeNode* constructFromPrePost(vector<int>& preorder, vector<int>& postorder) {
        int n = preorder.size();
        for (int i = 0; i < n; ++i) {
            post_map[postorder[i]] = i;
        }
        return build(preorder, 0, n - 1, postorder, 0, n - 1);
    }
};
