class Solution {
public:
    bool isMatch(string s, string p) {
        int s_idx = 0, p_idx = 0, star_idx = -1, s_tmp_idx = -1;
        while (s_idx < s.length()) {
            if (p_idx < p.length() && (p[p_idx] == '?' || p[p_idx] == s[s_idx])) {
                s_idx++; p_idx++;
            } else if (p_idx < p.length() && p[p_idx] == '*') {
                star_idx = p_idx++;
                s_tmp_idx = s_idx;
            } else if (star_idx != -1) {
                p_idx = star_idx + 1;
                s_idx = ++s_tmp_idx;
            } else return false;
        }
        while (p_idx < p.length() && p[p_idx] == '*') p_idx++;
        return p_idx == p.length();
    }
};
