class Solution {
public:
    vector<vector<int>> imageSmoother(vector<vector<int>>& img) {
        int m = img.size();
        int n = img[0].size();
        vector<vector<int>> dirs = {{0,1},{0,-1},{1,0},{-1,0},{-1,-1},{1,1},{-1,1},{1,-1}};
        vector<vector<int>> result(m, vector<int>(n, 0));
        
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                int sum = img[i][j];
                int count = 1;
                
                for (int k = 0; k < dirs.size(); k++) {
                    int x = i + dirs[k][0];
                    int y = j + dirs[k][1];
                    
                    if (x < 0 || x >= m || y < 0 || y >= n) continue;
                    
                    sum += img[x][y];
                    count++;
                }
                
                result[i][j] = sum / count;
            }
        }
        
        return result;
    }
};
