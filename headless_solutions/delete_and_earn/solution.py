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
    def deleteAndEarn(self, nums: List[int]) -> int:
        if not nums: return 0
        points = [0] * (max(nums) + 1)
        for num in nums:
            points[num] += num
            
        take, skip = 0, 0
        for val in points:
            take, skip = skip + val, max(take, skip)
        return max(take, skip)
