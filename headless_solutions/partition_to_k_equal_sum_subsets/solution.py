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
    def canPartitionKSubsets(self, nums: List[int], k: int) -> bool:
        total = sum(nums)
        if total % k != 0: return False
        target, n = total // k, len(nums)
        dp = [-1] * (1 << n)
        dp[0] = 0
        for mask in range(1 << n):
            if dp[mask] == -1: continue
            for i in range(n):
                if not (mask & (1 << i)) and dp[mask] + nums[i] <= target:
                    dp[mask | (1 << i)] = (dp[mask] + nums[i]) % target
        return dp[(1 << n) - 1] == 0
