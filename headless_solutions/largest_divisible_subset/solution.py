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
    def largestDivisibleSubset(self, nums: List[int]) -> List[int]:
        nums.sort()
        n = len(nums)
        dp, parent = [1] * n, [-1] * n
        max_idx = 0
        for i in range(1, n):
            for j in range(i):
                if nums[i] % nums[j] == 0 and dp[i] < dp[j] + 1:
                    dp[i] = dp[j] + 1
                    parent[i] = j
            if dp[i] > dp[max_idx]:
                max_idx = i
        res = []
        while max_idx != -1:
            res.append(nums[max_idx])
            max_idx = parent[max_idx]
        return res
