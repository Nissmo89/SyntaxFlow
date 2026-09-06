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
    def thirdMax(self, nums: List[int]) -> int:
        nums.sort(reverse=True)
        elemCounted = 1
        prevElem = nums[0]
        for i in range(1, len(nums)):
            if nums[i] != prevElem:
                elemCounted += 1
                prevElem = nums[i]
                if elemCounted == 3:
                    return nums[i]
        return nums[0]
