class Solution {
public:
    string reverseStr(string s, int k) {
        char arr[s.size()];
        strcpy(arr, s.c_str());
        int n = s.size();
        int i = 0;
        while (i < n) {
            int j = min(i + k - 1, n - 1);
            reverse(arr, i, j);
            i += 2 * k;
        }
        return string(arr);
    }
    void reverse(char* arr, int l, int r) {
        while (l < r) {
            char temp = arr[l];
            arr[l++] = arr[r];
            arr[r--] = temp;
        }
    }
};
