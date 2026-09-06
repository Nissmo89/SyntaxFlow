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

from collections import Counter

class Solution:
    def isPossible(self, nums: List[int]) -> bool:
        left = Counter(nums)
        end = Counter()
        for x in nums:
            if left[x] == 0: continue
            left[x] -= 1
            if end[x - 1] > 0:
                end[x - 1] -= 1
                end[x] += 1
            elif left[x + 1] > 0 and left[x + 2] > 0:
                left[x + 1] -= 1
                left[x + 2] -= 1
                end[x + 2] += 1
            else:
                return False
        return True
