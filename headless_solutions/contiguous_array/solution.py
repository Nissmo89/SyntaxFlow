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
    def findMaxLength(self, nums: List[int]) -> int:
        # Map to store the first occurrence of each count value.
        # Key: count, Value: index
        prefix_sum_map = {0: -1}  # Initialize with count 0 at index -1
        
        max_length = 0
        count = 0  # Running count: +1 for 1, -1 for 0
        
        for i in range(len(nums)):
            # Update count based on the current element
            if nums[i] == 1:
                count += 1
            else:  # nums[i] == 0
                count -= 1
            
            # Check if this count has been seen before
            if count in prefix_sum_map:
                # If seen, calculate the length of the subarray
                # from the previous occurrence of this count to the current index.
                prev_index = prefix_sum_map[count]
                max_length = max(max_length, i - prev_index)
            else:
                # If not seen, store the current index as the first occurrence
                prefix_sum_map[count] = i
                
        return max_length
