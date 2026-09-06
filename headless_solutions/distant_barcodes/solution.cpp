class Solution {
public:
    vector<int> rearrangeBarcodes(vector<int>& barcodes) {
        unordered_map<int, int> counts;
        for (int b : barcodes) counts[b]++;
        
        vector<pair<int, int>> sorted_counts;
        for (auto const& [val, freq] : counts) sorted_counts.push_back({freq, val});
        sort(sorted_counts.rbegin(), sorted_counts.rend());
        
        vector<int> res(barcodes.size());
        int pos = 0;
        for (auto const& [freq, val] : sorted_counts) {
            for (int i = 0; i < freq; ++i) {
                if (pos >= barcodes.size()) pos = 1;
                res[pos] = val;
                pos += 2;
            }
        }
        return res;
    }
};
