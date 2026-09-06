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
    def minStickers(self, stickers: List[str], target: str) -> int:
        memo = {"": 0}
        sticker_counts = [collections.Counter(s) for s in stickers]
        
        def solve(t):
            if t in memo: return memo[t]
            t_count = collections.Counter(t)
            res = float('inf')
            # Optimization: only use stickers that contain the first char of target
            first_char = t[0]
            for s_count in sticker_counts:
                if first_char in s_count:
                    # Create new target string
                    new_t = ""
                    for char, count in t_count.items():
                        new_t += char * max(0, count - s_count[char])
                    res = min(res, 1 + solve(new_t))
            memo[t] = res
            return res
            
        ans = solve(target)
        return ans if ans != float('inf') else -1
