class Solution {
public:
    vector<string> removeInvalidParentheses(string s) {
        int l = 0, r = 0;
        for (char c : s) {
            if (c == '(') l++;
            else if (c == ')') {
                if (l > 0) l--;
                else r++;
            }
        }
        vector<string> res;
        dfs(s, 0, l, r, res);
        return res;
    }
    void dfs(string s, int start, int l, int r, vector<string>& res) {
        if (l == 0 && r == 0 && isValid(s)) { res.push_back(s); return; }
        for (int i = start; i < s.size(); ++i) {
            if (i != start && s[i] == s[i - 1]) continue;
            if (s[i] == '(' || s[i] == ')') {
                string next = s.substr(0, i) + s.substr(i + 1);
                if (s[i] == '(' && l > 0) dfs(next, i, l - 1, r, res);
                else if (s[i] == ')' && r > 0) dfs(next, i, l, r - 1, res);
            }
        }
    }
    bool isValid(string s) {
        int count = 0;
        for (char c : s) {
            if (c == '(') count++;
            else if (c == ')') {
                count--;
                if (count < 0) return false;
            }
        }
        return count == 0;
    }
};
