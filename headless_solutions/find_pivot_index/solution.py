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
    def pivotIndex(self, nums: List[int]) -> int:
        prefixSum = sum(nums)
        
        leftSum = 0
        for i in range(len(nums)):
            if leftSum == prefixSum - leftSum - nums[i]:
                return i
            leftSum += nums[i]
        
        return -1
