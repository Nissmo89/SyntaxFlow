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
    def numMovesStonesII(self, stones: List[int]) -> List[int]:
        stones.sort()
        n = len(stones)
        low = n
        high = max(stones[-1] - stones[1] - n + 2, stones[-2] - stones[0] - n + 2)
        
        i = 0
        for j in range(n):
            while stones[j] - stones[i] >= n:
                i += 1
            count = j - i + 1
            if count == n - 1 and stones[j] - stones[i] == n - 2:
                low = min(low, 2)
            else:
                low = min(low, n - count)
        return [low, high]
