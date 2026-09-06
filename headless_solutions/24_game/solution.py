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
    def judgePoint24(self, cards: List[int]) -> bool:
        def solve(nums):
            if len(nums) == 1:
                return abs(nums[0] - 24) < 1e-6
            for i in range(len(nums)):
                for j in range(len(nums)):
                    if i == j: continue
                    next_nums = [nums[k] for k in range(len(nums)) if k != i and k != j]
                    a, b = nums[i], nums[j]
                    options = [a + b, a - b, a * b]
                    if abs(b) > 1e-6: options.append(a / b)
                    for res in options:
                        if solve(next_nums + [res]): return True
            return False
        return solve([float(c) for c in cards])
