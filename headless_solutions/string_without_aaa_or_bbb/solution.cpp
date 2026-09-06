class Solution {
public:
    string strWithout3a3b(int a, int b) {
        string res = "";
        while (a > 0 || b > 0) {
            int n = res.size();
            // If last two are same, force the other character
            if (n >= 2 && res[n - 1] == res[n - 2]) {
                if (res[n - 1] == 'a') {
                    res += 'b'; b--;
                } else {
                    res += 'a'; a--;
                }
            } else {
                // Otherwise, pick the one with more remaining count
                if (a >= b) {
                    res += 'a'; a--;
                } else {
                    res += 'b'; b--;
                }
            }
        }
        return res;
    }
};
