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
    def rob(self, nums: List[int]) -> int:
        prev2, prev1 = 0, 0
        for num in nums:
            # Current max is either skipping this house (prev1)
            # or robbing it (prev2 + num)
            prev2, prev1 = prev1, max(prev1, prev2 + num)
        return prev1
