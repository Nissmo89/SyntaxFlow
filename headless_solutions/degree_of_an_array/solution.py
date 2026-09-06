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
    def findShortestSubArray(self, nums: List[int]) -> int:
        # Dictionary to store the first occurrence index of each number
        left_idx = {}
        # Dictionary to store the last occurrence index of each number
        right_idx = {}
        # Dictionary to store the frequency (count) of each number
        counts = {}

        # First pass: Populate the dictionaries
        for i, num in enumerate(nums):
            # If num is not in left_idx, this is its first occurrence
            if num not in left_idx:
                left_idx[num] = i
            
            # Always update right_idx to store the latest (last) occurrence
            right_idx[num] = i
            
            # Increment the count for num
            counts[num] = counts.get(num, 0) + 1

        # Find the maximum frequency (degree of the array)
        degree = 0
        for count in counts.values():
            degree = max(degree, count)

        # Initialize min_len with the maximum possible length (length of nums itself)
        min_len = len(nums)

        # Second pass: Find the shortest subarray with the same degree
        for num, count in counts.items():
            # If this number's frequency matches the array's degree
            if count == degree:
                # Calculate the length of the subarray containing all occurrences of num
                current_len = right_idx[num] - left_idx[num] + 1
                # Update min_len if this subarray is shorter
                min_len = min(min_len, current_len)
        
        return min_len
