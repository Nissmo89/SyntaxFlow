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
    def reorganizeString(self, s: str) -> str:
        from collections import Counter
        n = len(s)
        count = Counter(s)
        # Sort by frequency
        chars = sorted(count.keys(), key=lambda x: count[x], reverse=True)
        
        if count[chars[0]] > (n + 1) // 2:
            return ""
            
        res = [''] * n
        idx = 0
        for char in chars:
            for _ in range(count[char]):
                if idx >= n:
                    idx = 1
                res[idx] = char
                idx += 2
        return "".join(res)
