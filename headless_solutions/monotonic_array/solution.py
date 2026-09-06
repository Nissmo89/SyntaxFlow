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
    def isMonotonic(self, nums: List[int]) -> bool:
        increasing = True
        decreasing = True
        for i in range(1, len(nums)):
            increasing &= nums[i - 1] <= nums[i]
            decreasing &= nums[i - 1] >= nums[i]
        return increasing or decreasing
