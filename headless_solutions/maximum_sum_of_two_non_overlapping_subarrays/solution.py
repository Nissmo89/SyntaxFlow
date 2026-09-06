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
    def maxSumTwoNoOverlap(self, nums: List[int], firstLen: int, secondLen: int) -> int:
        n = len(nums)
        prefix_sum = [0] * (n + 1)
        for i in range(n):
            prefix_sum[i + 1] = prefix_sum[i] + nums[i]
            
        def solve(L, M):
            res, maxL = 0, 0
            for i in range(L + M, n + 1):
                maxL = max(maxL, prefix_sum[i - M] - prefix_sum[i - M - L])
                res = max(res, maxL + prefix_sum[i] - prefix_sum[i - M])
            return res
            
        return max(solve(firstLen, secondLen), solve(secondLen, firstLen))
