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
    def lastStoneWeightII(self, stones: List[int]) -> int:
        total_sum = sum(stones)
        target = total_sum // 2
        dp = [False] * (target + 1)
        dp[0] = True
        
        for stone in stones:
            for i in range(target, stone - 1, -1):
                if dp[i - stone]:
                    dp[i] = True
                    
        for i in range(target, -1, -1):
            if dp[i]:
                return total_sum - 2 * i
        return 0
