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

from typing import List

class Solution:
    def maxSubArray(self, nums: List[int]) -> int:
        current_sum = global_max = nums[0]
        
        for x in nums[1:]:
            # Decide whether to extend the previous subarray or start fresh
            current_sum = max(x, current_sum + x)
            global_max = max(global_max, current_sum)
            
        return global_max
