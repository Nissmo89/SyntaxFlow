class Solution {
public:
    bool isAlienSorted(vector<string>& words, string order) {
        unordered_map<char, int> mapping;
        for (int i = 0; i < order.length(); i++) {
            mapping[order[i]] = i;
        }
        for (int i = 1; i < words.size(); i++) {
            if (!isSorted(words[i - 1], words[i], mapping)) {
                return false;
            }
        }
        return true;
    }

    bool isSorted(string &s1, string &s2, unordered_map<char, int> &mapping) {
        int n = s1.length(), m = s2.length();
        for (int i = 0; i < n && i < m; i++) {
            if (s1[i] != s2[i]) {
                if (mapping[s1[i]] < mapping[s2[i]]) {
                    return true;
                } else {
                    return false;
                }
            }
        }
        return n <= m;
    }
};
