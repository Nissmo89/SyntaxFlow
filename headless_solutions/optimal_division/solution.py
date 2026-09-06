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
    def optimalDivision(self, nums: List[int]) -> str:
        n = len(nums)
        if n == 1: return str(nums[0])
        if n == 2: return f"{nums[0]}/{nums[1]}"
        
        # Construct the string: nums[0] / (nums[1] / nums[2] / ... / nums[n-1])
        res = [str(nums[0]), "/(", str(nums[1])]
        for i in range(2, n):
            res.append("/" + str(nums[i]))
        res.append(")")
        return "".join(res)
