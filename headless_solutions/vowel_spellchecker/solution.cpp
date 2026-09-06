#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>

class Solution {
    string toLower(string s) {
        for (char &c : s) c = tolower(c);
        return s;
    }
    string toMask(string s) {
        string res = toLower(s);
        for (char &c : res) {
            if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') c = '#';
        }
        return res;
    }
public:
    vector<string> spellchecker(vector<string>& wordlist, vector<string>& queries) {
        unordered_set<string> exact(wordlist.begin(), wordlist.end());
        unordered_map<string, string> cap, vowel;
        for (const string& w : wordlist) {
            string l = toLower(w);
            string m = toMask(w);
            if (cap.find(l) == cap.end()) cap[l] = w;
            if (vowel.find(m) == vowel.end()) vowel[m] = w;
        }
        vector<string> res;
        for (const string& q : queries) {
            if (exact.count(q)) res.push_back(q);
            else if (cap.count(toLower(q))) res.push_back(cap[toLower(q)]);
            else if (vowel.count(toMask(q))) res.push_back(vowel[toMask(q)]);
            else res.push_back("");
        }
        return res;
    }
};
