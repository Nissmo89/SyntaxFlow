class Solution {
    unordered_map<string, int> memo;
    vector<vector<int>> stickerCounts;

public:
    int minStickers(vector<string>& stickers, string target) {
        int n = stickers.size();
        stickerCounts.assign(n, vector<int>(26, 0));
        for (int i = 0; i < n; ++i)
            for (char c : stickers[i]) stickerCounts[i][c - 'a']++;
        
        memo[""] = 0;
        int res = solve(target);
        return res > 1e9 ? -1 : res;
    }

    int solve(string target) {
        if (memo.count(target)) return memo[target];
        int n = stickerCounts.size();
        vector<int> tar(26, 0);
        for (char c : target) tar[c - 'a']++;
        
        int res = 1e9;
        for (int i = 0; i < n; ++i) {
            if (stickerCounts[i][target[0] - 'a'] == 0) continue;
            string nextTarget = "";
            for (int j = 0; j < 26; ++j) {
                if (tar[j] > stickerCounts[i][j])
                    nextTarget += string(tar[j] - stickerCounts[i][j], j + 'a');
            }
            int sub = solve(nextTarget);
            if (sub != 1e9) res = min(res, 1 + sub);
        }
        return memo[target] = res;
    }
};
