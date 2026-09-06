class Solution {
public:
    vector<int> prisonAfterNDays(vector<int>& cells, int n) {
        map<vector<int>, int> seen;
        while (n > 0) {
            seen[cells] = n--;
            vector<int> next(8, 0);
            for (int i = 1; i < 7; ++i)
                next[i] = (cells[i - 1] == cells[i + 1]) ? 1 : 0;
            cells = next;
            if (seen.count(cells)) {
                n %= (seen[cells] - n);
            }
        }
        return cells;
    }
};
