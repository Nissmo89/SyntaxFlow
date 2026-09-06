class Solution {
    vector<int> parent;
    int components;

    int find(int i) {
        if (parent[i] == i) return i;
        return parent[i] = find(parent[i]);
    }

    void unite(int i, int j) {
        int rootI = find(i), rootJ = find(j);
        if (rootI != rootJ) {
            parent[rootI] = rootJ;
            components--;
        }
    }

    bool isSimilar(const string& a, const string& b) {
        int diff = 0;
        for (int i = 0; i < a.size(); ++i) {
            if (a[i] != b[i] && ++diff > 2) return false;
        }
        return true;
    }

public:
    int numSimilarGroups(vector<string>& strs) {
        int n = strs.size();
        parent.resize(n);
        iota(parent.begin(), parent.end(), 0);
        components = n;
        for (int i = 0; i < n; ++i) {
            for (int j = i + 1; j < n; ++j) {
                if (isSimilar(strs[i], strs[j])) unite(i, j);
            }
        }
        return components;
    }
};
