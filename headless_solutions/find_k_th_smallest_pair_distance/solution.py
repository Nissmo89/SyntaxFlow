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
    def smallestDistancePair(self, nums: List[int], k: int) -> int:
        # Helper function to count pairs with distance <= max_dist
        def count_pairs(max_dist: int) -> int:
            count = 0
            n = len(nums)
            right = 0 # The right pointer for the sliding window

            # Iterate through each element as the 'left' element of a pair
            for left in range(n):
                # Move the 'right' pointer forward as long as the distance is within max_dist
                # and we haven't reached the end of the array.
                while right < n and nums[right] - nums[left] <= max_dist:
                    right += 1
                # For the current `nums[left]`, all elements from `nums[left + 1]` up to
                # `nums[right - 1]` form pairs with distance <= max_dist.
                # The number of such elements is `right - 1 - left`.
                count += (right - 1 - left)
            return count

        # 1. Sort the array to enable efficient counting.
        nums.sort()
        n = len(nums)

        # 2. Define the search space for the distance.
        # Minimum possible distance is 0.
        # Maximum possible distance is the difference between the largest and smallest elements.
        low = 0
        high = nums[n - 1] - nums[0]
        ans = high # Initialize answer to the maximum possible distance

        # 3. Binary search for the k-th smallest distance.
        while low <= high:
            mid = low + (high - low) // 2 # Candidate distance

            # Count how many pairs have a distance less than or equal to 'mid'.
            pair_count = count_pairs(mid)

            # If the count is less than k, it means 'mid' is too small.
            # The k-th smallest distance must be larger.
            if pair_count < k:
                low = mid + 1
            else:
                # If the count is k or more, 'mid' could be the answer,
                # or the answer could be a smaller distance.
                # We store 'mid' as a potential answer and try smaller distances.
                ans = mid # 'mid' is a possible answer
                high = mid - 1

        # The loop terminates when low > high. 'ans' holds the smallest distance 'd'
        # such that count_pairs(d) >= k.
        return ans
