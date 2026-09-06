#include <vector>
#include <string>

using namespace std;

class Solution {
public:
    int countPalindromicSubsequences(string s) {
        int n = s.length();
        long long MOD = 1e9 + 7;
        // dp[i][j] stores the number of distinct palindromic subsequences in s[i...j]
        vector<vector<long long>> dp(n, vector<long long>(n, 0));

        for (int i = 0; i < n; ++i) {
            dp[i][i] = 1;
        }

        for (int len = 2; len <= n; ++len) {
            for (int i = 0; i <= n - len; ++i) {
                int j = i + len - 1;
                if (s[i] == s[j]) {
                    int low = i + 1;
                    int high = j - 1;
                    
                    // Find the first and last occurrence of s[i] in (i, j)
                    while (low <= high && s[low]!= s[i]) low++;
                    while (low <= high && s[high]!= s[i]) high--;

                    if (low > high) {
                        // Case: no s[i] in between (e.g., "aa" or "aba")
                        // The palindromes are: 
                        // 1. Palindromes in s[i+1...j-1]
                        // 2. Palindromes in s[i+1...j-1] wrapped by s[i]
                        // 3. The single char s[i]
                        // 4. The double char s[i]s[j]
                        dp[i][j] = dp[i + 1][j - 1] * 2 + 2;
                    } else if (low == high) {
                        // Case: one s[i] in between (e.g., "aaa")
                        // We already counted the single char s[i] via dp[i+1][j-1]
                        // We add the double char s[i]s[j] and the wrapped ones
                        dp[i][j] = dp[i + 1][j - 1] * 2 + 1;
                    } else {
                        // Case: two or more s[i] in between (e.g., "aaaa")
                        // Subtract the inner duplicates
                        dp[i][j] = dp[i + 1][j - 1] * 2 - dp[low + 1][high - 1];
                    }
                } else {
                    dp[i][j] = dp[i + 1][j] + dp[i][j - 1] - dp[i + 1][j - 1];
                }
                dp[i][j] = (dp[i][j] + MOD) % MOD;
            }
        }

        return (int)dp[0][n - 1];
    }
};
