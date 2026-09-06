#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int minHeightShelves(vector<vector<int>>& books, int shelfWidth) {
        int n = books.size();
        vector<int> dp(n + 1, INT_MAX);
        dp[0] = 0;                         // no books → height 0

        for (int i = 1; i <= n; ++i) {
            int curWidth = 0;
            int curHeight = 0;
            // try to place books j..i on the same shelf
            for (int j = i; j >= 1; --j) {
                curWidth += books[j - 1][0];          // thickness
                if (curWidth > shelfWidth) break;     // cannot fit more
                curHeight = max(curHeight, books[j - 1][1]); // tallest book on this shelf
                dp[i] = min(dp[i], dp[j - 1] + curHeight);
            }
        }
        return dp[n];
    }
};
