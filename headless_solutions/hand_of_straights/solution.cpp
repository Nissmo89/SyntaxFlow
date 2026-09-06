#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    bool isNStraightHand(vector<int>& hand, int groupSize) {
        // Quick reject: total cards must be divisible by groupSize
        if (hand.size() % groupSize != 0) return false;

        // Frequency map (sorted by key)
        map<int, int> cnt;
        for (int x : hand) ++cnt[x];

        // Iterate over keys in ascending order
        for (auto it = cnt.begin(); it != cnt.end(); ++it) {
            int start = it->first;
            int need  = it->second;          // how many groups start here
            if (need == 0) continue;         // already consumed

            // Try to consume a block [start, start+groupSize-1]
            for (int i = 0; i < groupSize; ++i) {
                int key = start + i;
                auto jt = cnt.find(key);
                if (jt == cnt.end() || jt->second < need) return false;
                jt->second -= need;
            }
        }
        return true;
    }
};
