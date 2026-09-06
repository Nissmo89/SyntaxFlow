class Solution {
public:
    int numTilePossibilities(string tiles) {
        vector<int> count(26, 0);
        for (char c : tiles) count[c - 'A']++;
        return dfs(count);
    }

private:
    int dfs(vector<int>& count) {
        int sum = 0;
        for (int i = 0; i < 26; ++i) {
            if (count[i] > 0) {
                sum++; // Count the current sequence
                count[i]--;
                sum += dfs(count); // Add sequences formed by picking this char
                count[i]++; // Backtrack
            }
        }
        return sum;
    }
};
