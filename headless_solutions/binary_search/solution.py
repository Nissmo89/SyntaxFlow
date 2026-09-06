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
    def search(self, nums: List[int], target: int) -> int:
        left = 0
        right = len(nums) - 1 # Initialize right pointer to the last index

        # Continue searching as long as the search space [left, right] is valid
        while left <= right:
            # Calculate the middle index. Python handles large integers automatically.
            mid = left + (right - left) // 2 # Use integer division

            if nums[mid] == target:
                # Target found, return its index
                return mid
            elif nums[mid] < target:
                # If middle element is less than target,
                # target must be in the right half.
                # Discard mid and everything to its left.
                left = mid + 1
            else: # nums[mid] > target
                # If middle element is greater than target,
                # target must be in the left half.
                # Discard mid and everything to its right.
                right = mid - 1
        
        # If the loop finishes, target was not found in the array
        return -1
