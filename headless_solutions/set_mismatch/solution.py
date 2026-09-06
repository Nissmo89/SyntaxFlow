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
    def findErrorNums(self, nums: List[int]) -> List[int]:
        freq = {}
        dup = -1
        missing = -1
        
        for num in nums:
            freq[num] = freq.get(num, 0) + 1
            if freq[num] == 2:
                dup = num
        
        n = len(nums)
        expectedSum = n * (n + 1) // 2
        actualSum = sum(nums)
        
        missing = expectedSum - actualSum + dup
        
        return [dup, missing]
