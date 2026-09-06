class Solution {
    map<pair<int, int>, vector<TreeNode*>> memo;
    vector<TreeNode*> generate(int start, int end) {
        if (start > end) return {nullptr};
        if (memo.count({start, end})) return memo[{start, end}];
        
        vector<TreeNode*> res;
        for (int i = start; i <= end; ++i) {
            vector<TreeNode*> lefts = generate(start, i - 1);
            vector<TreeNode*> rights = generate(i + 1, end);
            for (auto l : lefts) {
                for (auto r : rights) {
                    res.push_back(new TreeNode(i, l, r));
                }
            }
        }
        return memo[{start, end}] = res;
    }
public:
    vector<TreeNode*> generateTrees(int n) {
        return generate(1, n);
    }
};
