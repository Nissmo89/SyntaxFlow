#include <vector>
#include <string>
#include <algorithm>
#include <iostream>

class Solution {
public:
    string largestTimeFromDigits(vector<int>& arr) {
        sort(arr.begin(), arr.end());
        string ans = "";
        do {
            int h = arr[0] * 10 + arr[1];
            int m = arr[2] * 10 + arr[3];
            if (h < 24 && m < 60) {
                string time = to_string(arr[0]) + to_string(arr[1]) + ":" + 
                              to_string(arr[2]) + to_string(arr[3]);
                if (time > ans) ans = time;
            }
        } while (next_permutation(arr.begin(), arr.end()));
        return ans;
    }
};
