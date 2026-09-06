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
    def sumSubseqWidths(self, nums: List[int]) -> int:
        nums.sort()
        mod = 10**9 + 7
        n = len(nums)
        res = 0
        p2 = 1
        for i in range(n):
            # nums[i] as max: nums[i] * 2^i
            # nums[i] as min: nums[i] * 2^(n-1-i)
            res = (res + nums[i] * p2 - nums[n - 1 - i] * p2) % mod
            p2 = (p2 * 2) % mod
        return res
