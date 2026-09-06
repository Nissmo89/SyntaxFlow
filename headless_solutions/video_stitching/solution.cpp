class Solution {
public:
    int videoStitching(vector<vector<int>>& clips, int time) {
        sort(clips.begin(), clips.end());
        int res = 0, st = 0, end = 0, i = 0, n = clips.size();
        while (st < time) {
            while (i < n && clips[i][0] <= st) {
                end = max(end, clips[i++][1]);
            }
            if (st == end) return -1; // Cannot advance
            st = end;
            res++;
        }
        return res;
    }
};
