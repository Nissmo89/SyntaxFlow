class Solution {
    int parent[26];
    int find(int i) {
        if (parent[i] == i) return i;
        return parent[i] = find(parent[i]);
    }
    void unite(int i, int j) {
        int rootI = find(i), rootJ = find(j);
        if (rootI != rootJ) {
            if (rootI < rootJ) parent[rootJ] = rootI;
            else parent[rootI] = rootJ;
        }
    }
public:
    string smallestEquivalentString(string s1, string s2, string baseStr) {
        for (int i = 0; i < 26; ++i) parent[i] = i;
        for (int i = 0; i < s1.length(); ++i) unite(s1[i] - 'a', s2[i] - 'a');
        for (char &c : baseStr) c = find(c - 'a') + 'a';
        return baseStr;
    }
};
