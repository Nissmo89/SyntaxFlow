class Solution {
public:
    bool validMountainArray(vector<int>& arr) {
        int n = arr.size();
        int l = 0, r = n - 1;
        while (l + 1 < n && arr[l] < arr[l + 1]) l++;
        while (r > 0 && arr[r - 1] > arr[r]) r--;
        return 0 < l && l == r && r < n - 1;
    }
};
