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
    def splitArraySameAverage(self, nums: List[int]) -> bool:
        n, S = len(nums), sum(nums)
        m = n // 2
        left = [set() for _ in range(m + 1)]
        left[0].add(0)
        for x in nums[:m]:
            for i in range(m - 1, -1, -1):
                for s in left[i]: left[i + 1].add(s + x)
        right = [set() for _ in range(n - m + 1)]
        right[0].add(0)
        for x in nums[m:]:
            for i in range(n - m - 1, -1, -1):
                for s in right[i]: right[i + 1].add(s + x)
        for k in range(1, n // 2 + 1):
            if (S * k) % n == 0:
                target = (S * k) // n
                for i in range(max(0, k - (n - m)), min(k, m) + 1):
                    for s in left[i]:
                        if (target - s) in right[k - i]: return True
        return False
