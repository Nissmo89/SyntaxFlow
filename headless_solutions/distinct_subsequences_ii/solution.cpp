#include <string>
#include <vector>
#include <numeric>

class Solution {
public:
    int distinctSubseqII(std::string s) {
        long long MOD = 1e9 + 7;
        // endsWith[i] stores the count of distinct subsequences ending with character ('a' + i)
        std::vector<long long> endsWith(26, 0);
        
        // total_sum stores the sum of all elements in endsWith, representing the total number of distinct subsequences found so far.
        long long total_sum = 0;
        
        for (char c : s) {
            int char_index = c - 'a';
            
            // The number of new subsequences ending with 'c' is:
            // (total_sum of existing distinct subsequences) + 1 (for the subsequence 'c' itself)
            // We need to subtract the old count of subsequences ending with 'c' to avoid double counting.
            // The number of new distinct subsequences ending with 'c' is (total_sum + 1).
            long long new_endsWith_c = (total_sum + 1) % MOD;
            
            // Store the old count of subsequences ending with 'c' to update total_sum correctly.
            long long old_endsWith_c = endsWith[char_index];
            
            // Update the count for subsequences ending with 'c'.
            endsWith[char_index] = new_endsWith_c;
            
            // Update the total_sum.
            // The change in total_sum is (new_endsWith_c - old_endsWith_c).
            // We add MOD before taking modulo to handle potential negative results from subtraction.
            total_sum = (total_sum - old_endsWith_c + new_endsWith_c + MOD) % MOD;
        }
        
        // The final answer is the total sum of distinct subsequences.
        return static_cast<int>(total_sum);
    }
};
