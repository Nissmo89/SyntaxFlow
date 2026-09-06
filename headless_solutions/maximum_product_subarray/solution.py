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
    def maxProduct(self, nums: List[int]) -> int:
        global_max = cur_max = cur_min = nums[0]
        
        for i in range(1, len(nums)):
            if nums[i] < 0:
                cur_max, cur_min = cur_min, cur_max
            
            cur_max = max(nums[i], cur_max * nums[i])
            cur_min = min(nums[i], cur_min * nums[i])
            
            global_max = max(global_max, cur_max)
            
        return global_max
