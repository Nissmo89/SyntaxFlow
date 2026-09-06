class Solution {
    vector<string> mapping = {"", "", "abc", "def", "ghi", "jkl", "mno", "pqrs", "tuv", "wxyz"};
public:
    vector<string> letterCombinations(string digits) {
        if (digits.empty()) return {};
        vector<string> res;
        string current;
        backtrack(digits, 0, current, res);
        return res;
    }
    void backtrack(const string& digits, int idx, string& current, vector<string>& res) {
        if (idx == digits.size()) {
            res.push_back(current);
            return;
        }
        string letters = mapping[digits[idx] - '0'];
        for (char c : letters) {
            current.push_back(c);
            backtrack(digits, idx + 1, current, res);
            current.pop_back();
        }
    }
};
