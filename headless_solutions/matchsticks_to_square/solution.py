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
    def makesquare(self, matchsticks: List[int]) -> bool:
        total = sum(matchsticks)
        if total % 4 != 0: return False
        target = total // 4
        matchsticks.sort(reverse=True)
        sides = [0] * 4
        
        def dfs(idx):
            if idx == len(matchsticks):
                return True
            for i in range(4):
                if sides[i] + matchsticks[idx] <= target:
                    sides[i] += matchsticks[idx]
                    if dfs(idx + 1): return True
                    sides[i] -= matchsticks[idx]
                if sides[i] == 0: break
            return False
            
        return dfs(0)
