class Solution {
public:
    int heightChecker(vector<int>& heights) {
        int count[101] = {0};
        for (int height : heights) {
            count[height]++;
        }
        int res = 0;
        int h_ptr = 1;
        for (int height : heights) {
            while (count[h_ptr] == 0) {
                h_ptr++;
            }
            if (h_ptr != height) {
                res++;
            }
            count[h_ptr]--;
        }
        return res;
    }
};
