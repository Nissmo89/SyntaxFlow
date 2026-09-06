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
    def nextGreaterElements(self, nums: List[int]) -> List[int]:
        n = len(nums)
        res = [-1] * n
        stack = []
        for i in range(2 * n):
            val = nums[i % n]
            while stack and nums[stack[-1]] < val:
                res[stack.pop()] = val
            if i < n:
                stack.append(i)
        return res
