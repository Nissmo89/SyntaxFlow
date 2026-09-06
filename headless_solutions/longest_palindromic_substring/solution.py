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
    def longestPalindrome(self, s: str) -> str:
        if not s: return ""
        start, max_len = 0, 0
        
        def expand(l, r):
            while l >= 0 and r < len(s) and s[l] == s[r]:
                l -= 1
                r += 1
            return r - l - 1
            
        for i in range(len(s)):
            len1 = expand(i, i)
            len2 = expand(i, i + 1)
            curr_max = max(len1, len2)
            if curr_max > max_len:
                max_len = curr_max
                start = i - (curr_max - 1) // 2
                
        return s[start : start + max_len]
