// C++ implementation matching signature
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int getMaxRepetitions(string s1, int n1, string s2, int n2) {
        // Map: position in s2 -> (count1, count2)
        unordered_map<int, pair<long long, long long>> record;
        long long count1 = 0, count2 = 0;   // how many s1 / s2 processed
        int i = 0, j = 0;                  // indices in s1 / s2

        while (count1 < n1) {
            if (s1[i] == s2[j]) {
                ++j;
                if (j == (int)s2.size()) {   // finished one s2
                    j = 0;
                    ++count2;
                }
            }
            ++i;
            if (i == (int)s1.size()) {       // finished one s1
                i = 0;
                ++count1;
            }

            // cycle detection
            if (record.find(j) != record.end()) {
                auto [prevCount1, prevCount2] = record[j];
                long long preLoopCount1 = prevCount1;
                long long preLoopCount2 = prevCount2;

                long long loopLen = count1 - prevCount1;   // copies of s1 in one cycle
                long long loopCnt = count2 - prevCount2;   // copies of s2 in one cycle

                long long remaining = n1 - preLoopCount1;
                long long fullCycles = remaining / loopLen;

                long long totalCount2 = preLoopCount2
                                       + fullCycles * loopCnt
                                       + (remaining % loopLen) * (loopCnt / loopLen);

                return (int)(totalCount2 / n2);
            } else {
                record[j] = {count1, count2};
            }
        }
        return (int)(count2 / n2);
    }
};
