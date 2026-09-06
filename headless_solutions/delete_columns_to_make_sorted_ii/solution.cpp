class Solution {
public:
    int minDeletionSize(vector<string>& strs) {
        int n = strs.size(), m = strs[0].size();
        int deletions = 0;
        vector<bool> is_sorted(n - 1, false);
        
        for (int j = 0; j < m; ++j) {
            bool must_delete = false;
            for (int i = 0; i < n - 1; ++i) {
                if (!is_sorted[i] && strs[i][j] > strs[i+1][j]) {
                    must_delete = true;
                    break;
                }
            }
            
            if (must_delete) {
                deletions++;
            } else {
                for (int i = 0; i < n - 1; ++i) {
                    if (strs[i][j] < strs[i+1][j]) is_sorted[i] = true;
                }
            }
        }
        return deletions;
    }
};
