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
    def maxWidthRamp(self, nums: List[int]) -> int:
        stack = []
        for i, val in enumerate(nums):
            if not stack or nums[stack[-1]] > val:
                stack.append(i)
        
        max_width = 0
        for j in range(len(nums) - 1, -1, -1):
            while stack and nums[stack[-1]] <= nums[j]:
                max_width = max(max_width, j - stack.pop())
        return max_width
