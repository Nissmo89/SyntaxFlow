class Solution {
public:
    bool buddyStrings(string s, string goal) {
        if (s.length() != goal.length()) return false;
        if (s == goal) {
            set<char> unique_chars(s.begin(), s.end());
            return unique_chars.size() < s.size();
        }
        vector<int> dif;
        for (int i = 0; i < s.length(); ++i) if (s[i] != goal[i]) dif.push_back(i);
        return dif.size() == 2 && s[dif[0]] == goal[dif[1]] && s[dif[1]] == goal[dif[0]];
    }
};
