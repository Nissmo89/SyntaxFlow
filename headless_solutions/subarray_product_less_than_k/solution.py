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
    def numSubarrayProductLessThanK(self, nums: List[int], k: int) -> int:
        if k <= 1:
            return 0
        count, product, left = 0, 1, 0
        for right, val in enumerate(nums):
            product *= val
            while product >= k and left <= right:
                product //= nums[left]
                left += 1
            count += (right - left + 1)
        return count
