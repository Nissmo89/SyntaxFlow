class Solution {
    unordered_map<string, int> serialToId;
    unordered_map<int, int> idToCount;
    vector<TreeNode*> res;
    int curId = 1;

    int postorder(TreeNode* root) {
        if (!root) return 0;
        string serial = to_string(postorder(root->left)) + "," + 
                        to_string(root->val) + "," + 
                        to_string(postorder(root->right));
        if (serialToId.find(serial) == serialToId.end()) serialToId[serial] = curId++;
        int id = serialToId[serial];
        if (++idToCount[id] == 2) res.push_back(root);
        return id;
    }

public:
    vector<TreeNode*> findDuplicateSubtrees(TreeNode* root) {
        postorder(root);
        return res;
    }
};
