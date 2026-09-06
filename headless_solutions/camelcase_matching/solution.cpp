// C++ implementation matching signature
class Solution {
public:
    vector<bool> camelMatch(vector<string>& queries, string pattern) {
        vector<bool> result;
        result.reserve(queries.size());          // avoid reallocations

        for (const string& q : queries) {
            bool ok = true;
            size_t j = 0;                        // index in pattern

            for (char c : q) {
                if (j < pattern.size() && c == pattern[j]) {
                    ++j;                         // matched a pattern char
                } else if (c >= 'A' && c <= 'Z') {
                    ok = false;                  // unmatched capital letter
                    break;
                }
                // lower‑case letters are ignored automatically
            }
            result.push_back(ok && j == pattern.size());
        }
        return result;
    }
};
