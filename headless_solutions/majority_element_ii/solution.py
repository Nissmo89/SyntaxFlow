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
    def majorityElement(self, nums: List[int]) -> List[int]:
        cand1, cand2, count1, count2 = 0, 1, 0, 0
        for n in nums:
            if n == cand1: count1 += 1
            elif n == cand2: count2 += 1
            elif count1 == 0: cand1, count1 = n, 1
            elif count2 == 0: cand2, count2 = n, 1
            else: count1, count2 = count1 - 1, count2 - 1
        
        res = []
        for c in [cand1, cand2]:
            if nums.count(c) > len(nums) // 3:
                res.append(c)
        return list(set(res))
