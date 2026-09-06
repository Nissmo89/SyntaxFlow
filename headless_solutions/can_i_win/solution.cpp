// C++ implementation matching signature
class Solution {
public:
    bool canIWin(int maxChoosableInteger, int desiredTotal) {
        if (desiredTotal <= 0) return true;
        int maxSum = (maxChoosableInteger + 1) * maxChoosableInteger / 2;
        if (maxSum < desiredTotal) return false;
        if (maxChoosableInteger >= desiredTotal) return true;

        int fullMask = 1 << maxChoosableInteger;
        vector<int> memo(fullMask, -1);          // -1: unknown, 0: lose, 1: win

        function<bool(int, int)> dfs = [&](int mask, int rem) -> bool {
            if (memo[mask] != -1) return memo[mask];
            for (int i = 0; i < maxChoosableInteger; ++i) {
                if (!(mask & (1 << i))) {
                    int val = i + 1;
                    if (val >= rem) return memo[mask] = 1;
                    if (!dfs(mask | (1 << i), rem - val))
                        return memo[mask] = 1;
                }
            }
            return memo[mask] = 0;
        };

        return dfs(0, desiredTotal);
    }
};
