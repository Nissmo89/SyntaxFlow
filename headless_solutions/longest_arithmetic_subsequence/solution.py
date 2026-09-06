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
    def longestArithSeqLength(self, nums: List[int]) -> int:
        n = len(nums)
        if n <= 2: return n
        dp = [{} for _ in range(n)]
        res = 2
        for i in range(n):
            for j in range(i):
                diff = nums[i] - nums[j]
                # If diff exists in dp[j], extend it, else start new pair (len 2)
                length = dp[j].get(diff, 1) + 1
                dp[i][diff] = length
                res = max(res, length)
        return res
