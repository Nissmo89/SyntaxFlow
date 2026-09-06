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

# Python 3 implementation matching signature
class Solution:
    def canIWin(self, maxChoosableInteger: int, desiredTotal: int) -> bool:
        if desiredTotal <= 0:
            return True
        max_sum = (maxChoosableInteger + 1) * maxChoosableInteger // 2
        if max_sum < desiredTotal:
            return False
        if maxChoosableInteger >= desiredTotal:
            return True

        memo = {}
        def dfs(mask: int, rem: int) -> bool:
            if mask in memo:
                return memo[mask]
            for i in range(maxChoosableInteger):
                if not (mask >> i) & 1:
                    val = i + 1
                    if val >= rem:
                        memo[mask] = True
                        return True
                    if not dfs(mask | (1 << i), rem - val):
                        memo[mask] = True
                        return True
            memo[mask] = False
            return False

        return dfs(0, desiredTotal)
