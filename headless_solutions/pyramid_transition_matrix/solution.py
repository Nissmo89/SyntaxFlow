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

from collections import defaultdict

class Solution:
    def pyramidTransition(self, bottom: str, allowed: List[str]) -> bool:
        mapping = defaultdict(list)
        for s in allowed:
            mapping[s[:2]].append(s[2])
            
        def solve(curr, nxt, idx):
            if len(curr) == 1: return True
            if idx == len(curr) - 1: return solve(nxt, "", 0)
            
            pair = curr[idx:idx+2]
            if pair not in mapping: return False
            
            for char in mapping[pair]:
                if solve(curr, nxt + char, idx + 1):
                    return True
            return False
            
        return solve(bottom, "", 0)
