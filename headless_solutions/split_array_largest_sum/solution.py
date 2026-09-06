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
    def splitArray(self, nums: List[int], k: int) -> int:
        def can_split(target):
            count, current_sum = 1, 0
            for num in nums:
                current_sum += num
                if current_sum > target:
                    current_sum = num
                    count += 1
            return count <= k

        left, right = max(nums), sum(nums)
        while left < right:
            mid = (left + right) // 2
            if can_split(mid):
                right = mid
            else:
                left = mid + 1
        return left
