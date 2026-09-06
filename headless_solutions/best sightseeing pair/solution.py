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
    def maxScoreIndices(self, nums: list[int]) -> list[int]:
        n = len(nums)
        prefix_sum = [0] * (n + 1)
        for i in range(n):
            prefix_sum[i + 1] = prefix_sum[i] + nums[i]
        max_score = 0
        max_indices = []
        for i in range(n + 1):
            if prefix_sum[i] == 0:
                max_score = n
                max_indices.append(i)
        return max_indices
