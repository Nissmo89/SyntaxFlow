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

from collections import deque

class Solution:
    def shortestSubarray(self, nums: list[int], k: int) -> int:
        n = len(nums)
        P = [0] * (n + 1)
        for i in range(n):
            P[i + 1] = P[i] + nums[i]
            
        res = n + 1
        dq = deque()
        for i in range(n + 1):
            while dq and P[i] - P[dq[0]] >= k:
                res = min(res, i - dq.popleft())
            while dq and P[i] <= P[dq[-1]]:
                dq.pop()
            dq.append(i)
            
        return res if res <= n else -1
