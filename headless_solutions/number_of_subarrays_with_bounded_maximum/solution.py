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
    def numSubarrayBoundedMax(self, nums: List[int], left: int, right: int) -> int:
        res, last_invalid, last_candidate = 0, -1, -1
        for i, x in enumerate(nums):
            if x > right: last_invalid = i
            if x >= left: last_candidate = i
            res += (last_candidate - last_invalid)
        return res
