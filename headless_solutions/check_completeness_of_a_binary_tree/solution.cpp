/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode() : val(0), left(nullptr), right(nullptr) {}
 *     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
 *     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
 * };
 */
class Solution {
public:
    bool isCompleteTree(TreeNode* root) {
        // BFS queue
        std::queue<TreeNode*> q;
        q.push(root);
        bool seenNull = false;               // have we met a missing child yet?
        
        while (!q.empty()) {
            TreeNode* cur = q.front();
            q.pop();
            
            if (cur == nullptr) {
                seenNull = true;             // first gap encountered
            } else {
                if (seenNull) return false;  // non‑null after a gap → not complete
                q.push(cur->left);
                q.push(cur->right);
            }
        }
        return true;                         // no violations found
    }
};
