class Solution {
public:
    vector<int> maxDepthAfterSplit(string seq) {
        vector<int> res(seq.length());
        int depth = 0;
        for (int i = 0; i < seq.length(); ++i) {
            if (seq[i] == '(') {
                res[i] = depth++ % 2;
            } else {
                res[i] = --depth % 2;
            }
        }
        return res;
    }
};
