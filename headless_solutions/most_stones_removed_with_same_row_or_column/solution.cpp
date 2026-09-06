class Solution {
    unordered_map<int, int> parent;
    int components = 0;

    int find(int i) {
        if (parent.find(i) == parent.end()) {
            parent[i] = i;
            components++;
        }
        if (parent[i] == i) return i;
        return parent[i] = find(parent[i]);
    }

    void unite(int i, int j) {
        int rootI = find(i);
        int rootJ = find(j);
        if (rootI != rootJ) {
            parent[rootI] = rootJ;
            components--;
        }
    }

public:
    int removeStones(vector<vector<int>>& stones) {
        for (auto& s : stones) {
            unite(s[0], s[1] + 10001);
        }
        return stones.size() - components;
    }
};
