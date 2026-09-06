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
    def bestRotation(self, nums: List[int]) -> int:
        n = len(nums)
        change = [0] * n
        for i, val in enumerate(nums):
            change[(i - val + 1) % n] -= 1
        
        max_score, best_k, current_score = -float('inf'), 0, 0
        for k in range(n):
            current_score += change[k] + 1
            if current_score > max_score:
                max_score = current_score
                best_k = k
        return best_k
