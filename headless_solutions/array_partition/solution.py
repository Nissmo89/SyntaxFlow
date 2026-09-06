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

from typing import List

class Solution:
    def arrayPairSum(self, nums: List[int]) -> int:
        # Step 1: Sort the array in non-decreasing order.
        # This ensures that adjacent elements are as close as possible,
        # minimizing the "loss" from the larger element in each pair.
        nums.sort()

        maximized_sum = 0
        # Step 2: Iterate through the sorted array and sum every second element.
        # These elements (at indices 0, 2, 4, ...) will always be the minimum
        # of their respective pairs (nums[i], nums[i+1]).
        for i in range(0, len(nums), 2):
            maximized_sum += nums[i]
            
        return maximized_sum
