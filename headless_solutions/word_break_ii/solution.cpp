class Solution {
    unordered_map<string, vector<string>> memo;
public:
    vector<string> wordBreak(string s, vector<string>& wordDict) {
        unordered_set<string> dict(wordDict.begin(), wordDict.end());
        return dfs(s, dict);
    }

    vector<string> dfs(string s, unordered_set<string>& dict) {
        if (memo.count(s)) return memo[s];
        vector<string> res;
        if (s.empty()) {
            res.push_back("");
            return res;
        }
        for (const string& word : dict) {
            if (s.substr(0, word.size()) == word) {
                vector<string> sub = dfs(s.substr(word.size()), dict);
                for (const string& str : sub) {
                    res.push_back(word + (str.empty() ? "" : " ") + str);
                }
            }
        }
        return memo[s] = res;
    }
};
