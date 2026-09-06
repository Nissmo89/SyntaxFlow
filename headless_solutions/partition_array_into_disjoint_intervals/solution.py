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
    def partitionDisjoint(self, nums: List[int]) -> int:
        partition_idx = 0
        max_left = nums[0]
        curr_max = nums[0]
        
        for i in range(1, len(nums)):
            curr_max = max(curr_max, nums[i])
            if nums[i] < max_left:
                partition_idx = i
                max_left = curr_max
        
        return partition_idx + 1
