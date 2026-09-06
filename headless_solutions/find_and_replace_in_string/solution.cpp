class Solution {
public:
    string findReplaceString(string s, vector<int>& indices, vector<string>& sources, vector<string>& targets) {
        unordered_map<int, int> lookup;
        for (int i = 0; i < indices.size(); ++i) {
            if (s.substr(indices[i], sources[i].length()) == sources[i]) {
                lookup[indices[i]] = i;
            }
        }
        string res = "";
        for (int i = 0; i < s.length(); ) {
            if (lookup.count(i)) {
                res += targets[lookup[i]];
                i += sources[lookup[i]].length();
            } else {
                res += s[i++];
            }
        }
        return res;
    }
};
