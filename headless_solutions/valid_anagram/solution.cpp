class Solution {
public:
    bool isAnagram(string s, string t) {
        if (s.length() != t.length()) return false;
        
        int alphabet[26] = {0};
        for (char c : s) alphabet[c - 'a']++;
        for (char c : t) alphabet[c - 'a']--;
        
        for (int count : alphabet) {
            if (count != 0) return false;
        }
        
        return true;
    }
};
