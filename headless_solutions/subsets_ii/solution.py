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
    def subsetsWithDup(self, nums: List[int]) -> List[List[int]]:
        nums.sort()
        res = []
        def backtrack(start, path):
            res.append(list(path))
            for i in range(start, len(nums)):
                if i > start and nums[i] == nums[i-1]:
                    continue
                path.append(nums[i])
                backtrack(i + 1, path)
                path.pop()
        backtrack(0, [])
        return res
