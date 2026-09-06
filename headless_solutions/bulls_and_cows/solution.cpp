class Solution {
public:
    string getHint(string secret, string guess) {
        int bulls = 0;
        int cntSecret[10] = {0};
        int cntGuess[10] = {0};
        int n = secret.size();
        for (int i = 0; i < n; ++i) {
            if (secret[i] == guess[i]) {
                ++bulls;
            } else {
                int d = secret[i] - '0';
                int g = guess[i] - '0';
                ++cntSecret[d];
                ++cntGuess[g];
            }
        }
        int cows = 0;
        for (int i = 0; i < 10; ++i) {
            cows += min(cntSecret[i], cntGuess[i]);
        }
        return to_string(bulls) + "A" + to_string(cows) + "B";
    }
};
