class Solution {
public:
    int expressiveWords(string s, vector<string>& words) {
        int count = 0;
        for (const string& word : words) {
            if (isStretchy(s, word)) count++;
        }
        return count;
    }

private:
    bool isStretchy(const string& s, const string& word) {
        int i = 0, j = 0;
        while (i < s.size() && j < word.size()) {
            if (s[i] != word[j]) return false;
            int len1 = getGroupLength(s, i);
            int len2 = getGroupLength(word, j);
            if (len1 < len2 || (len1 > len2 && len1 < 3)) return false;
            i += len1;
            j += len2;
        }
        return i == s.size() && j == word.size();
    }

    int getGroupLength(const string& str, int start) {
        int end = start;
        while (end < str.size() && str[end] == str[start]) end++;
        return end - start;
    }
};
