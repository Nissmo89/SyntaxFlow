class Solution {
public:
    int countArrangement(int n) {
        int bm = 0, pl = n;
        return dfs(bm, pl);
    }

    int dfs(int bm, int pl) {
        if (pl == 0) return 1;
        int S = 0;
        for (int i = 1; i <= n; i++) {
            if (!(bm & (1 << (i - 1))) && ((i % pl == 0) || (pl % i == 0))) {
                S += dfs(bm | (1 << (i - 1)), pl - 1);
            }
        }
        return S;
    }
};
