class Solution {
public:
    int numMatchingSubseq(string s, vector<string>& words) {
        vector<vector<pair<int, int>>> buckets(26);
        for (int i = 0; i < words.size(); ++i) {
            buckets[words[i][0] - 'a'].push_back({i, 0});
        }
        int count = 0;
        for (char c : s) {
            vector<pair<int, int>> current = buckets[c - 'a'];
            buckets[c - 'a'].clear();
            for (auto& p : current) {
                if (p.second + 1 == words[p.first].size()) count++;
                else buckets[words[p.first][p.second + 1] - 'a'].push_back({p.first, p.second + 1});
            }
        }
        return count;
    }
};
