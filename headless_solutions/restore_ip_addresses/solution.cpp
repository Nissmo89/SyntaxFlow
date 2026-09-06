class Solution {
public:
    vector<string> restoreIpAddresses(string s) {
        vector<string> res;
        backtrack(s, 0, {}, res);
        return res;
    }
    
    void backtrack(string& s, int start, vector<string> path, vector<string>& res) {
        if (path.size() == 4) {
            if (start == s.length()) res.push_back(path[0] + "." + path[1] + "." + path[2] + "." + path[3]);
            return;
        }
        for (int len = 1; len <= 3 && start + len <= s.length(); ++len) {
            string sub = s.substr(start, len);
            if ((sub.size() > 1 && sub[0] == '0') || stoi(sub) > 255) continue;
            path.push_back(sub);
            backtrack(s, start + len, path, res);
            path.pop_back();
        }
    }
};
