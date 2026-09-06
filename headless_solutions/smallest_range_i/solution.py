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
    def smallestRangeI(self, nums: List[int], k: int) -> int:
        M, m = max(nums), min(nums)
        diff, extension = M - m, 2 * k
        if diff <= extension:
            return 0
        else:
            return diff - extension
