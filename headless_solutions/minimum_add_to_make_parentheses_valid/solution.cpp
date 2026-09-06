class Solution {
public:
    int minAddToMakeValid(string s) {
        int open_needed = 0;
        int balance = 0;
        for (char c : s) {
            if (c == '(') {
                balance++;
            } else {
                if (balance > 0) balance--;
                else open_needed++;
            }
        }
        return open_needed + balance;
    }
};
