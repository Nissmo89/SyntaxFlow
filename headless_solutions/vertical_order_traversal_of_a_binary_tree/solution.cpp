class Solution {
public:
    vector<vector<int>> verticalTraversal(TreeNode* root) {
        vector<tuple<int, int, int>> nodes;
        queue<pair<TreeNode*, pair<int, int>>> q;
        q.push({root, {0, 0}});
        
        while(!q.empty()){
            auto [node, pos] = q.front(); q.pop();
            int r = pos.first, c = pos.second;
            nodes.emplace_back(c, r, node->val);
            if(node->left) q.push({node->left, {r + 1, c - 1}});
            if(node->right) q.push({node->right, {r + 1, c + 1}});
        }
        
        sort(nodes.begin(), nodes.end());
        
        vector<vector<int>> res;
        for(int i = 0; i < nodes.size(); ++i){
            if(i == 0 || get<0>(nodes[i]) != get<0>(nodes[i-1])) res.push_back({});
            res.back().push_back(get<2>(nodes[i]));
        }
        return res;
    }
};
