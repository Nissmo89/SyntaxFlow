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
    def sortArrayByParity(self, nums: List[int]) -> List[int]:
        beg, end = 0, len(nums) - 1
        while beg <= end:
            if nums[beg] % 2 == 0:
                beg += 1
            else:
                nums[beg], nums[end] = nums[end], nums[beg]
                end -= 1
        return nums
