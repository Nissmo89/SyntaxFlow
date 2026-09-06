class Solution {
    vector<int> parent;
    int count;
    int find(int i) {
        if (parent[i] == i) return i;
        return parent[i] = find(parent[i]);
    }
    void unite(int i, int j) {
        int rootI = find(i), rootJ = find(j);
        if (rootI != rootJ) {
            parent[rootI] = rootJ;
            count--;
        }
    }
public:
    int minSwapsCouples(vector<int>& row) {
        int n = row.size() / 2;
        parent.resize(n);
        iota(parent.begin(), parent.end(), 0);
        count = n;
        for (int i = 0; i < n; ++i) {
            unite(row[2 * i] / 2, row[2 * i + 1] / 2);
        }
        return n - count;
    }
};
