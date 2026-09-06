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
    def searchRange(self, nums: List[int], target: int) -> List[int]:
        def find_bound(is_first: bool) -> int:
            low, high, ans = 0, len(nums) - 1, -1
            while low <= high:
                mid = (low + high) // 2
                if nums[mid] == target:
                    ans = mid
                    if is_first: high = mid - 1
                    else: low = mid + 1
                elif nums[mid] < target: low = mid + 1
                else: high = mid - 1
            return ans
        return [find_bound(True), find_bound(False)]
