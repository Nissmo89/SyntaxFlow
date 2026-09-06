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
    def maxRotateFunction(self, nums: List[int]) -> int:
        n = len(nums)
        total_sum = sum(nums)
        f = sum(i * val for i, val in enumerate(nums))
        max_f = f
        
        for i in range(1, n):
            # Recurrence: F(k) = F(k-1) + sum - n * nums[n-k]
            f = f + total_sum - n * nums[n - i]
            max_f = max(max_f, f)
            
        return max_f
