class Solution {
public:
    vector<int> pancakeSort(vector<int>& arr) {
        vector<int> res;
        for (int target = arr.size(); target > 0; --target) {
            int i = 0;
            while (arr[i] != target) i++;
            if (i == target - 1) continue;
            if (i > 0) {
                reverse(arr.begin(), arr.begin() + i + 1);
                res.push_back(i + 1);
            }
            reverse(arr.begin(), arr.begin() + target);
            res.push_back(target);
        }
        return res;
    }
};
