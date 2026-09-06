class Solution {
public:
    vector<vector<int>> palindromePairs(vector<string>& words) {
        unordered_map<string, int> dict;
        for (int i = 0; i < words.size(); ++i) dict[words[i]] = i;
        vector<vector<int>> res;
        for (int i = 0; i < words.size(); ++i) {
            string word = words[i];
            for (int j = 0; j <= word.size(); ++j) {
                string left = word.substr(0, j);
                string right = word.substr(j);
                if (isPalindrome(left)) {
                    string revRight = string(right.rbegin(), right.rend());
                    if (dict.count(revRight) && dict[revRight] != i) res.push_back({dict[revRight], i});
                }
                if (j != word.size() && isPalindrome(right)) {
                    string revLeft = string(left.rbegin(), left.rend());
                    if (dict.count(revLeft) && dict[revLeft] != i) res.push_back({i, dict[revLeft]});
                }
            }
        }
        return res;
    }
    bool isPalindrome(const string& s) {
        int l = 0, r = s.size() - 1;
        while (l < r) if (s[l++] != s[r--]) return false;
        return true;
    }
};
