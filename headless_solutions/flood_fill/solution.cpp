class Solution {
public:
    vector<vector<int>> floodFill(vector<vector<int>>& image, int sr, int sc, int color) {
        int originalColor = image[sr][sc];
        dfs(image, sr, sc, originalColor, color);
        return image;
    }

    void dfs(vector<vector<int>>& image, int i, int j, int originalColor, int newColor) {
        if (i < 0 || i >= image.size() || j < 0 || j >= image[0].size() || image[i][j] == newColor || image[i][j] != originalColor) {
            return;
        }
        image[i][j] = newColor;
        dfs(image, i - 1, j, originalColor, newColor);
        dfs(image, i + 1, j, originalColor, newColor);
        dfs(image, i, j - 1, originalColor, newColor);
        dfs(image, i, j + 1, originalColor, newColor);
    }
};
