import collections
import functools
import itertools
import math
import heapq
import bisect
from typing import *
from collections import *
from functools import *
from heapq import *
from bisect import *

class Solution:
    def distinctSubseqII(self, s: str) -> int:
        MOD = 10**9 + 7
        # endsWith[i] stores the count of distinct subsequences ending with character ('a' + i)
        endsWith = [0] * 26
        
        # total_sum stores the sum of all elements in endsWith, representing the total number of distinct subsequences found so far.
        total_sum = 0
        
        for char in s:
            char_index = ord(char) - ord('a')
            
            # The number of new subsequences ending with 'char' is:
            # (total_sum of existing distinct subsequences) + 1 (for the subsequence 'char' itself)
            # We need to subtract the old count of subsequences ending with 'char' to avoid double counting.
            # The number of new distinct subsequences ending with 'char' is (total_sum + 1).
            new_endsWith_char = (total_sum + 1) % MOD
            
            # Store the old count of subsequences ending with 'char' to update total_sum correctly.
            old_endsWith_char = endsWith[char_index]
            
            # Update the count for subsequences ending with 'char'.
            endsWith[char_index] = new_endsWith_char
            
            # Update the total_sum.
            # The change in total_sum is (new_endsWith_char - old_endsWith_char).
            # We add MOD before taking modulo to handle potential negative results from subtraction.
            total_sum = (total_sum - old_endsWith_char + new_endsWith_char + MOD) % MOD
        
        # The final answer is the total sum of distinct subsequences.
        return total_sum
