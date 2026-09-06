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
    def maxSubarraySumCircular(self, nums: List[int]) -> int:
        total, cur_max, max_sum = 0, 0, nums[0]
        cur_min, min_sum = 0, nums[0]
        for x in nums:
            cur_max = max(cur_max + x, x)
            max_sum = max(max_sum, cur_max)
            cur_min = min(cur_min + x, x)
            min_sum = min(min_sum, cur_min)
            total += x
        return max(max_sum, total - min_sum) if max_sum > 0 else max_sum
