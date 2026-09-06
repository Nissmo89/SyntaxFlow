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
    def findMin(self, nums: List[int]) -> int:
        low = 0
        high = len(nums) - 1

        # The loop continues as long as the search space has more than one element.
        # When low == high, we have found the minimum element.
        while low < high:
            # Calculate mid point to avoid potential overflow
            mid = low + (high - low) // 2

            # Case 1: nums[mid] > nums[high]
            # The minimum element must be in the right half (mid + 1 to high).
            # Example: [4, 5, 6, 7, 0, 1, 2], mid=7, high=2. 7 > 2, so min is in [0, 1, 2].
            if nums[mid] > nums[high]:
                low = mid + 1
            # Case 2: nums[mid] < nums[high]
            # The minimum element is either nums[mid] or in the left half (low to mid).
            # Example: [6, 7, 0, 1, 2, 4, 5], mid=0, high=5. 0 < 5, so min is in [6, 7, 0].
            elif nums[mid] < nums[high]:
                high = mid  # mid could be the minimum, so we include it.
            # Case 3: nums[mid] == nums[high]
            # We cannot determine if the minimum is to the left or right of mid.
            # Example: [3, 1, 3, 3, 3], mid=3, high=3. Min is 1 (left).
            # Example: [3, 3, 3, 1, 3], mid=3, high=3. Min is 1 (right).
            # In this case, we safely discard nums[high] by decrementing high.
            # This is because nums[high] is a duplicate of nums[mid], and if it were
            # the unique minimum, nums[mid] would have been strictly smaller.
            else: # nums[mid] == nums[high]
                high -= 1

        # When the loop terminates, low == high, and this index points to the minimum element.
        return nums[low]
