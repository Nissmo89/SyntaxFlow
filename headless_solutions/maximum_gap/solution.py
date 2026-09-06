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
    def maximumGap(self, nums: List[int]) -> int:
        n = len(nums)
        if n < 2: return 0
        min_val, max_val = min(nums), max(nums)
        if min_val == max_val: return 0
        
        bucket_size = max(1, (max_val - min_val) // (n - 1))
        num_buckets = (max_val - min_val) // bucket_size + 1
        
        min_b = [float('inf')] * num_buckets
        max_b = [-1] * num_buckets
        
        for x in nums:
            idx = (x - min_val) // bucket_size
            min_b[idx] = min(min_b[idx], x)
            max_b[idx] = max(max_b[idx], x)
            
        max_gap, prev_max = 0, min_val
        for i in range(num_buckets):
            if max_b[i] == -1: continue
            max_gap = max(max_gap, min_b[i] - prev_max)
            prev_max = max_b[i]
        return max_gap
