class Solution {
public:
    int minDominoRotations(vector<int>& tops, vector<int>& bottoms) {
        auto check = [&](int target) {
            int topRot = 0, botRot = 0;
            for (int i = 0; i < tops.size(); ++i) {
                if (tops[i] != target && bottoms[i] != target) return -1;
                if (tops[i] != target) topRot++;
                if (bottoms[i] != target) botRot++;
            }
            return min(topRot, botRot);
        };

        int res = check(tops[0]);
        if (res != -1) return res;
        return check(bottoms[0]);
    }
};
