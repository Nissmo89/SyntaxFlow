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
    def dominantIndex(self, nums: List[int]) -> int:
        max_num = float('-inf')
        max_index = -1
        for i, num in enumerate(nums):
            if num > max_num:
                max_num = num
                max_index = i
        for i, num in enumerate(nums):
            if i != max_index and num > 0 and num * 2 > max_num:
                return -1
        return max_index
