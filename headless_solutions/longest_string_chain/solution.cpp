class Solution {
public:
    int longestStrChain(vector<string>& words) {
        sort(words.begin(), words.end(), [](const string& a, const string& b) {
            return a.length() < b.length();
        });
        unordered_map<string, int> dp;
        int maxChain = 0;
        for (const string& word : words) {
            dp[word] = 1;
            for (int i = 0; i < word.length(); ++i) {
                string prev = word.substr(0, i) + word.substr(i + 1);
                if (dp.count(prev)) {
                    dp[word] = max(dp[word], dp[prev] + 1);
                }
            }
            maxChain = max(maxChain, dp[word]);
        }
        return maxChain;
    }
};
