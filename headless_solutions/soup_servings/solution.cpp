#include <vector>
#include <cmath>
#include <algorithm>

class Solution {
    double memo[200][200];
    double dp(int a, int b) {
        if (a <= 0 && b <= 0) return 0.5;
        if (a <= 0) return 1.0;
        if (b <= 0) return 0.0;
        if (memo[a][b] > 0) return memo[a][b];
        return memo[a][b] = 0.25 * (dp(a - 4, b) + dp(a - 3, b - 1) + dp(a - 2, b - 2) + dp(a - 1, b - 3));
    }
public:
    double soupServings(int n) {
        if (n > 5000) return 1.0;
        int servings = (n + 24) / 25;
        for(int i=0; i<200; ++i) for(int j=0; j<200; ++j) memo[i][j] = 0;
        return dp(servings, servings);
    }
};
