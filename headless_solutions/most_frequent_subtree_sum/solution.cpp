class Solution {
    unordered_map<int, int> counts;
    int maxFreq = 0;

    int dfs(TreeNode* node) {
        if (!node) return 0;
        int sum = node->val + dfs(node->left) + dfs(node->right);
        maxFreq = max(maxFreq, ++counts[sum]);
        return sum;
    }

public:
    vector<int> findFrequentTreeSum(TreeNode* root) {
        dfs(root);
        vector<int> res;
        for (auto const& [sum, freq] : counts) {
            if (freq == maxFreq) res.push_back(sum);
        }
        return res;
    }
};
