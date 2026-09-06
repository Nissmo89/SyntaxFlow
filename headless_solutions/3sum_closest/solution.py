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

import math

class Solution:
    def threeSumClosest(self, nums: list[int], target: int) -> int:
        # Sort the array to enable the two-pointer approach.
        nums.sort()
        n = len(nums)
        
        # Initialize closest_sum with the sum of the first three elements.
        # Since n >= 3, this is always a valid initial sum.
        closest_sum = nums[0] + nums[1] + nums[2]
        
        # Iterate through the array, fixing the first element of the triplet.
        # We go up to n - 2 because we need at least two more elements (left and right).
        for i in range(n - 2):
            left = i + 1  # Pointer for the second element
            right = n - 1 # Pointer for the third element
            
            # Use two pointers to find the remaining two elements
            while left < right:
                current_sum = nums[i] + nums[left] + nums[right]
                
                # If the current sum is exactly the target, we found the closest possible sum.
                if current_sum == target:
                    return target
                
                # Update closest_sum if the current sum is closer to target
                # We compare absolute differences to find which sum is closer.
                if abs(current_sum - target) < abs(closest_sum - target):
                    closest_sum = current_sum
                
                # Adjust pointers based on whether the current sum is too small or too large
                if current_sum < target:
                    # Sum is too small, need to increase it. Move left pointer forward.
                    left += 1
                else: # current_sum > target
                    # Sum is too large, need to decrease it. Move right pointer backward.
                    right -= 1
                    
        return closest_sum
