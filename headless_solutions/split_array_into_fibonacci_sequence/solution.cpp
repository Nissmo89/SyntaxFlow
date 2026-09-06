class Solution {
public:
    vector<int> splitIntoFibonacci(string num) {
        vector<int> res;
        backtrack(num, res, 0);
        return res;
    }

    bool backtrack(string& s, vector<int>& res, int idx) {
        if (idx == s.size() && res.size() >= 3) return true;
        
        for (int i = idx; i < s.size(); ++i) {
            if (s[idx] == '0' && i > idx) break;
            long long val = stoll(s.substr(idx, i - idx + 1));
            if (val > INT_MAX) break;
            
            int sz = res.size();
            if (sz >= 2 && val > (long long)res[sz - 1] + res[sz - 2]) break;
            
            if (sz <= 1 || val == (long long)res[sz - 1] + res[sz - 2]) {
                res.push_back((int)val);
                if (backtrack(s, res, i + 1)) return true;
                res.pop_back();
            }
        }
        return false;
    }
};
