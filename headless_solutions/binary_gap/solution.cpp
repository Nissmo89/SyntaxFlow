class Solution {
public:
    int binaryGap(int n) {
        int maxDistance = 0;
        int lastPos = -1;
        int pos = 0;
        while (n > 0) {
            if (n % 2 == 1) {
                if (lastPos != -1) {
                    maxDistance = max(maxDistance, pos - lastPos);
                }
                lastPos = pos;
            }
            n /= 2;
            pos++;
        }
        return maxDistance;
    }
};
