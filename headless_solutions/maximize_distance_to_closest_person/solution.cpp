class Solution {
public:
    int maxDistToClosest(vector<int>& seats) {
        int n = seats.size();
        int last = -1, max_dist = 0;
        for (int i = 0; i < n; ++i) {
            if (seats[i] == 1) {
                if (last == -1) max_dist = i;
                else max_dist = max(max_dist, (i - last) / 2);
                last = i;
            }
        }
        return max(max_dist, n - 1 - last);
    }
};
