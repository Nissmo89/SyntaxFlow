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
    def numDecodings(self, s: str) -> int:
        MOD = 10**9 + 7
        # dp[i] represents ways to decode string of length i
        # prev2 is dp[i-2], prev1 is dp[i-1]
        prev2, prev1 = 1, (9 if s[0] == '*' else (1 if s[0] != '0' else 0))
        
        for i in range(1, len(s)):
            curr = 0
            # Single digit
            if s[i] == '*': curr = (prev1 * 9) % MOD
            elif s[i] != '0': curr = prev1
            
            # Two digits
            if s[i-1:i+1] == '**': curr = (curr + prev2 * 15) % MOD
            elif s[i-1] == '*': curr = (curr + prev2 * (2 if s[i] <= '6' else 1)) % MOD
            elif s[i] == '*': curr = (curr + prev2 * (9 if s[i-1] == '1' else 6 if s[i-1] == '2' else 0)) % MOD
            elif '10' <= s[i-1:i+1] <= '26': curr = (curr + prev2) % MOD
            
            prev2, prev1 = prev1, curr
        return prev1
