/**
 * @param {string} s
 * @return {number}
 */
var distinctSubseqII = function(s) {
    const MOD = 1_000_000_007;
    // endsWith[i] stores the count of distinct subsequences ending with character ('a' + i)
    const endsWith = new Array(26).fill(0);
    
    // total_sum stores the sum of all elements in endsWith, representing the total number of distinct subsequences found so far.
    let total_sum = 0;
    
    for (let i = 0; i < s.length; i++) {
        const char = s[i];
        const char_index = char.charCodeAt(0) - 'a'.charCodeAt(0);
        
        // The number of new subsequences ending with 'char' is:
        // (total_sum of existing distinct subsequences) + 1 (for the subsequence 'char' itself)
        // We need to subtract the old count of subsequences ending with 'char' to avoid double counting.
        // The number of new distinct subsequences ending with 'char' is (total_sum + 1).
        const new_endsWith_char = (total_sum + 1) % MOD;
        
        // Store the old count of subsequences ending with 'char' to update total_sum correctly.
        const old_endsWith_char = endsWith[char_index];
        
        // Update the count for subsequences ending with 'char'.
        endsWith[char_index] = new_endsWith_char;
        
        // Update the total_sum.
        // The change in total_sum is (new_endsWith_char - old_endsWith_char).
        // We add MOD before taking modulo to handle potential negative results from subtraction.
        total_sum = (total_sum - old_endsWith_char + new_endsWith_char + MOD) % MOD;
    }
    
    // The final answer is the total sum of distinct subsequences.
    return total_sum;
};
