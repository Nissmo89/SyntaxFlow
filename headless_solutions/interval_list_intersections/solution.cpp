class Solution {
public:
    vector<vector<int>> intervalIntersection(vector<vector<int>>& firstList, vector<vector<int>>& secondList) {
        vector<vector<int>> result;
        int i = 0, j = 0;
        while (i < firstList.size() && j < secondList.size()) {
            int start = max(firstList[i][0], secondList[j][0]);
            int end = min(firstList[i][1], secondList[j][1]);
            
            if (start <= end) {
                result.push_back({start, end});
            }
            
            // Move the pointer of the interval that ends first
            if (firstList[i][1] < secondList[j][1]) i++;
            else j++;
        }
        return result;
    }
};
